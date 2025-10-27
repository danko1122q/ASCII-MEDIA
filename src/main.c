/*
 * ASCII Image Converter - Terminal Image Viewer
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
#include <stdlib.h>

#include "../include/image.h"
#include "../include/print_image.h"
#include "../include/argparse.h"


int main(int argc, char* argv[]) {
    // Parses arguments (also sets up signal handlers)
    args_t args = parse_args(argc, argv);
    if (args.file_path == NULL)
        return 1;
    
    // Check for shutdown request
    if (g_shutdown_requested) {
        fprintf(stderr, "\n[+] ASCII-MEDIA terminated safely.\n");
        return 0;
    }

    // Check if file is GIF and animate flag is set
    if (is_gif_file(args.file_path) && args.animate_gif) {
        // Load and play animated GIF
        gif_animation_t anim = load_gif_animation(args.file_path);
        if (anim.frame_count > 0) {
            play_gif_animation(&anim, &args);
            free_gif_animation(&anim);
        } else {
            fprintf(stderr, "Warning: Could not load GIF animation, falling back to static image\n");
            // Fall through to static image loading
        }
    }
    
    // If not animated or animation failed, load as static image
    if (!args.animate_gif || !is_gif_file(args.file_path)) {
        // Loads image
        image_t original = load_image(args.file_path);
        if (!original.data)
            return 1;

        // Apply sharpening if requested
        if (args.sharpen_strength > 0.0) {
            unsharp_mask(&original, args.sharpen_strength, 1.0);
        }
        
        // Resizes image
        image_t resized = make_resized(&original, args.max_width, args.max_height, args.character_ratio);
        if (!resized.data) {
            free_image(&original);
            return 1;
        }
        
        // Use new args-based print function for enhanced features
        args_t print_args = args;
        print_image(&resized, print_args.edge_threshold, print_args.use_retro_colors, print_args.use_braille, print_args.use_grayscale);
        
        free_image(&original);
        free_image(&resized);
    }
    
    // Check if shutdown was requested during processing
    if (g_shutdown_requested) {
        fprintf(stderr, "\n[+] ASCII-MEDIA terminated safely.\n");
    }

    return 0;
}
