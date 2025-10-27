/*
 * ASCII Image Converter - Argument Parser
 * 
 * Copyright (c) 2025 danko12
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <string.h>

#include <sys/ioctl.h>
#include <unistd.h>
#include <signal.h>
#include "../include/argparse.h"

// Global variables for signal handling
static volatile sig_atomic_t g_terminal_resized = 0;
volatile sig_atomic_t g_shutdown_requested = 0;

// SIGWINCH handler - terminal resize detection
static void sigwinch_handler(int sig) {
    (void)sig;
    g_terminal_resized = 1;
}

// SIGINT handler - graceful shutdown (async-signal-safe)
static void sigint_handler(int sig) {
    (void)sig;
    g_shutdown_requested = 1;
    // Don't call fprintf or exit here - not async-signal-safe
    // Main loop will handle cleanup and message
}


#define DEFAULT_MAX_WIDTH 64
#define DEFAULT_MAX_HEIGHT 48
#define DEFAULT_CHARACTER_RATIO 2.0
#define DEFAULT_EDGE_THRESHOLD 4.0
#define DEFAULT_SHARPEN_STRENGTH 0.0
#define VERSION "3.0.0"

// Dimension Presets (Width x Height)
// Heights are adjusted for 4:1 aspect ratio correction (terminal characters are ~2x taller than wide)
#define PRESET_TINY_WIDTH 40
#define PRESET_TINY_HEIGHT 15
#define PRESET_SMALL_WIDTH 64
#define PRESET_SMALL_HEIGHT 24
#define PRESET_MEDIUM_WIDTH 100
#define PRESET_MEDIUM_HEIGHT 37
#define PRESET_LARGE_WIDTH 150
#define PRESET_LARGE_HEIGHT 56
#define PRESET_XLARGE_WIDTH 200
#define PRESET_XLARGE_HEIGHT 75
#define PRESET_XXLARGE_WIDTH 250
#define PRESET_XXLARGE_HEIGHT 93


// Setup signal handlers for terminal resize and graceful shutdown
void setup_signal_handlers(void) {
    struct sigaction sa_winch, sa_int;
    
    // SIGWINCH for terminal resize
    memset(&sa_winch, 0, sizeof(sa_winch));
    sa_winch.sa_handler = sigwinch_handler;
    sigemptyset(&sa_winch.sa_mask);
    sa_winch.sa_flags = SA_RESTART;
    sigaction(SIGWINCH, &sa_winch, NULL);
    
    // SIGINT for graceful shutdown  
    memset(&sa_int, 0, sizeof(sa_int));
    sa_int.sa_handler = sigint_handler;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;
    sigaction(SIGINT, &sa_int, NULL);
}

int terminal_was_resized(void) {
    if (g_terminal_resized) {
        g_terminal_resized = 0;
        return 1;
    }
    return 0;
}

void print_version(void) {
    printf("ASCII-MEDIA v%s\n", VERSION);
    printf("Copyright (c) 2025 danko12\n");
    printf("Advanced multimedia ASCII converter - Images, GIFs, Videos, Webcam\n");
    printf("Linux & macOS only\n");
}

void print_help(char* exec_alias) {
    print_version();
    printf("\nUSAGE:\n");
    printf("\t%s <path/to/image> [OPTIONS]\n\n", exec_alias);

    printf("ARGUMENTS:\n");
    printf("\t<path/to/image>\t\tPath to image file\n\n");

    printf("OPTIONS:\n");
    printf("\t-D <preset>\t\tDimension preset (1-6):\n");
    printf("\t\t\t\t  1 = Tiny    (40×15)   - Quick preview\n");
    printf("\t\t\t\t  2 = Small   (64×24)   - Default size\n");
    printf("\t\t\t\t  3 = Medium  (100×37)  - Standard detail\n");
    printf("\t\t\t\t  4 = Large   (150×56)  - High detail\n");
    printf("\t\t\t\t  5 = XLarge  (200×75)  - Very high detail\n");
    printf("\t\t\t\t  6 = XXLarge (250×93)  - Maximum detail\n");
    printf("\t-mw <width>\t\tMaximum width in characters (default: terminal width OR %d)\n", DEFAULT_MAX_WIDTH);
    printf("\t-mh <height>\t\tMaximum height in characters (default: terminal height OR %d)\n", DEFAULT_MAX_HEIGHT);
    printf("\t-et <threshold>\t\tEdge detection threshold, range: 0.0 - 4.0 (default: %.1f, disabled)\n", DEFAULT_EDGE_THRESHOLD);
    printf("\t-cr <ratio>\t\tHeight-to-width ratio for characters (default: %.1f)\n", DEFAULT_CHARACTER_RATIO);
    printf("\t-s, --sharpen <strength>\tSharpening strength, range: 0.0 - 2.0 (default: %.1f, disabled)\n", DEFAULT_SHARPEN_STRENGTH);
    printf("\t--retro-colors\t\tUse 3-bit retro color palette (8 colors) instead of 24-bit truecolor\n");
    printf("\t--braille\t\tUse braille characters for higher detail (experimental)\n");
    printf("\t--animate\t\tAnimate GIF files (if supported)\n");
    printf("\t--grayscale\t\tConvert image/GIF to black and white (grayscale mode)\n");
    printf("\t--enhanced-palette\tUse 70+ character precision palette for maximum detail\n");
    printf("\t--debug\t\t\tEnable debug mode with real-time stats (FPS, terminal size, etc)\n");
    printf("\t-h, --help\t\tShow this help message\n");
    printf("\t-v, --version\t\tShow version information\n");
    printf("\nNOTE: -D preset overrides -mw and -mh values. Use -mw/-mh for custom dimensions.\n");
}

void apply_dimension_preset(args_t* args, int preset) {
    switch(preset) {
        case 1:  // Tiny
            args->max_width = PRESET_TINY_WIDTH;
            args->max_height = PRESET_TINY_HEIGHT;
            break;
        case 2:  // Small
            args->max_width = PRESET_SMALL_WIDTH;
            args->max_height = PRESET_SMALL_HEIGHT;
            break;
        case 3:  // Medium
            args->max_width = PRESET_MEDIUM_WIDTH;
            args->max_height = PRESET_MEDIUM_HEIGHT;
            break;
        case 4:  // Large
            args->max_width = PRESET_LARGE_WIDTH;
            args->max_height = PRESET_LARGE_HEIGHT;
            break;
        case 5:  // XLarge
            args->max_width = PRESET_XLARGE_WIDTH;
            args->max_height = PRESET_XLARGE_HEIGHT;
            break;
        case 6:  // XXLarge
            args->max_width = PRESET_XXLARGE_WIDTH;
            args->max_height = PRESET_XXLARGE_HEIGHT;
            break;
        default:
            fprintf(stderr, "Warning: Invalid dimension preset %d. Valid range: 1-6. Using default.\n", preset);
            break;
    }
}

// Get size of terminal in characters. Returns 1 if successful.
// POSIX implementation for Linux/macOS
int try_get_terminal_size(size_t* width, size_t* height) {
    if (!isatty(0))
        return 0;
    
    struct winsize ws;
    if (ioctl(0, TIOCGWINSZ, &ws) == 0) {
        *width = (size_t) ws.ws_col;
        *height = (size_t) ws.ws_row;
        return 1;
    }
    
    return 0;
}


args_t parse_args(int argc, char* argv[]) {
    // Get variable defaults
    args_t args = {
        .file_path = NULL,
        .max_width = DEFAULT_MAX_WIDTH,
        .max_height = DEFAULT_MAX_HEIGHT,
        .character_ratio = DEFAULT_CHARACTER_RATIO,
        .edge_threshold = DEFAULT_EDGE_THRESHOLD,
        .use_retro_colors = 0,
        .sharpen_strength = DEFAULT_SHARPEN_STRENGTH,
        .use_braille = 0,
        .animate_gif = 0,
        .dimension_preset = 0,
        .use_grayscale = 0,
        .debug_mode = 0,
        .use_enhanced_palette = 0,
    };
    
    // Setup signal handlers for resize and shutdown
    setup_signal_handlers();

    try_get_terminal_size(&args.max_width, &args.max_height);

    // If no file given
    if (argc == 1) {
        print_help(argv[0]);
        return args;
    }

    // Check for help or version
    if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) {
        print_help(argv[0]);
        return args;
    } else if (!strcmp(argv[1], "-v") || !strcmp(argv[1], "--version")) {
        print_version();
        return args;
    } else {
        args.file_path = argv[1];
    }

    // Get optional parameters
    for (size_t i = 2; i < (size_t) argc; i++) {
        if (!strcmp(argv[i], "-D") && i + 1 < (size_t) argc) {
            args.dimension_preset = atoi(argv[++i]);
            apply_dimension_preset(&args, args.dimension_preset);
        }
        else if (!strcmp(argv[i], "-mw") && i + 1 < (size_t) argc)
            args.max_width = (size_t) atoi(argv[++i]);
        else if (!strcmp(argv[i], "-mh") && i + 1 < (size_t) argc)
            args.max_height = (size_t) atoi(argv[++i]);
        else if (!strcmp(argv[i], "-et") && i + 1 < (size_t) argc)
            args.edge_threshold = atof(argv[++i]);
        else if (!strcmp(argv[i], "-cr") && i + 1 < (size_t) argc)
            args.character_ratio = atof(argv[++i]);
        else if ((!strcmp(argv[i], "-s") || !strcmp(argv[i], "--sharpen")) && i + 1 < (size_t) argc)
            args.sharpen_strength = atof(argv[++i]);
        else if (!strcmp(argv[i], "--retro-colors"))
            args.use_retro_colors = 1;
        else if (!strcmp(argv[i], "--braille"))
            args.use_braille = 1;
        else if (!strcmp(argv[i], "--animate"))
            args.animate_gif = 1;
        else if (!strcmp(argv[i], "--grayscale"))
            args.use_grayscale = 1;
        else if (!strcmp(argv[i], "--enhanced-palette"))
            args.use_enhanced_palette = 1;
        else if (!strcmp(argv[i], "--debug"))
            args.debug_mode = 1;
        else if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
            print_help(argv[0]);
            args.file_path = NULL;
            return args;
        } else if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--version")) {
            print_version();
            args.file_path = NULL;
            return args;
        } else
            fprintf(stderr, "Warning: Ignoring invalid or incomplete argument '%s'\n", argv[i]);
    }

    return args;
}
