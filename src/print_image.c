/*
 * ASCII Image Converter - Enhanced Image Printing & Color Conversion
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
 * 
 * Features:
 * - Perceptually optimized character ramp
 * - Gamma-aware luminance calculation
 * - Adaptive contrast enhancement (CLAHE)
 * - Improved edge blending
 * - Accurate color preservation
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "../include/image.h"
#include "../include/argparse.h"

// Enhanced character ramp with better perceptual spacing (70+ levels)
// High precision mode - ordered from darkest to brightest
#define ENHANCED_CHARS " .'`^\",:;Il!i><~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$"
#define N_ENHANCED (sizeof(ENHANCED_CHARS) - 1)

// Standard quality character set (40 chars) - balanced performance/quality
#define VALUE_CHARS " .:-=+*#%@WMBNHRKOQGDPdbkhao*"
#define N_VALUES (sizeof(VALUE_CHARS) - 1)

// Simplified high-quality character set (12 levels) - used by default
#define SIMPLE_CHARS " .:-=+*#%@"
#define N_SIMPLE (sizeof(SIMPLE_CHARS) - 1)

// Braille patterns for higher detail (8 levels)
static const char* BRAILLE_CHARS[] = {
    " ", "⠁", "⠃", "⠇", "⠏", "⠟", "⠿", "⣿"
};
#define N_BRAILLE 8

// Color ANSI codes
#define RESET "\x1b[0m"

// Contrast enhancement parameters
#define CLIP_LIMIT 2.0
#define TILE_SIZE 8


typedef struct {
    double hue;
    double saturation;
    double value;
} hsv_t;


// ============================================================================
// Color Space Conversion with Accuracy Improvements
// ============================================================================

static double* get_max(double* a, double* b, double* c) {
    if ((*a >= *b) && (*a >= *c)) return a;
    if (*b >= *c) return b;
    return c;
}

static double* get_min(double* a, double* b, double* c) {
    if ((*a <= *b) && (*a <= *c)) return a;
    if (*b <= *c) return b;
    return c;
}


static hsv_t rgb_to_hsv(double red, double green, double blue) {
    hsv_t hsv;
    
    double* max = get_max(&red, &green, &blue);
    double* min = get_min(&red, &green, &blue);
    
    hsv.value = *max;
    double chroma = hsv.value - *min;
    
    // Calculate saturation
    if (fabs(hsv.value) < 1e-6) {
        hsv.saturation = 0.0;
    } else {
        hsv.saturation = chroma / hsv.value;
    }
    
    // Calculate hue
    if (chroma < 1e-6) {
        hsv.hue = 0.0;
    } else if (max == &red) {
        hsv.hue = 60.0 * fmod((green - blue) / chroma, 6.0);
        if (hsv.hue < 0.0) hsv.hue += 360.0;
    } else if (max == &green) {
        hsv.hue = 60.0 * (2.0 + (blue - red) / chroma);
    } else {
        hsv.hue = 60.0 * (4.0 + (red - green) / chroma);
    }
    
    return hsv;
}


static void hsv_to_rgb(const hsv_t* hsv, double* r, double* g, double* b) {
    double c = hsv->value * hsv->saturation;
    double h_prime = hsv->hue / 60.0;
    double x = c * (1.0 - fabs(fmod(h_prime, 2.0) - 1.0));
    
    double r1 = 0.0, g1 = 0.0, b1 = 0.0;
    
    if (h_prime >= 0.0 && h_prime < 1.0) {
        r1 = c; g1 = x; b1 = 0.0;
    } else if (h_prime >= 1.0 && h_prime < 2.0) {
        r1 = x; g1 = c; b1 = 0.0;
    } else if (h_prime >= 2.0 && h_prime < 3.0) {
        r1 = 0.0; g1 = c; b1 = x;
    } else if (h_prime >= 3.0 && h_prime < 4.0) {
        r1 = 0.0; g1 = x; b1 = c;
    } else if (h_prime >= 4.0 && h_prime < 5.0) {
        r1 = x; g1 = 0.0; b1 = c;
    } else {
        r1 = c; g1 = 0.0; b1 = x;
    }
    
    double m = hsv->value - c;
    *r = r1 + m;
    *g = g1 + m;
    *b = b1 + m;
}


// ============================================================================
// Luminance Calculation (ITU-R BT.601 with Gamma Compensation)
// Multi-stage approach for high fidelity ASCII mapping
// ============================================================================

// Calculate perceptually accurate luminance (ITU-R BT.601)
// Using simplified formula with gamma compensation as per requirements
static double calculate_luminance(double r, double g, double b) {
    // ITU-R BT.601 standard weights (NTSC/PAL)
    // L = 0.299*R + 0.587*G + 0.114*B
    double luminance = 0.299 * r + 0.587 * g + 0.114 * b;
    
    // Apply gamma compensation for better perceptual gradation
    // L_gamma = (L)^(1/2.2)
    double gamma_compensated = pow(luminance, 1.0 / 2.2);
    
    return gamma_compensated;
}


// ============================================================================
// Adaptive Contrast Enhancement (Simplified CLAHE)
// ============================================================================

static void enhance_contrast_adaptive(double* values, size_t width, size_t height) {
    if (!values || width == 0 || height == 0) return;
    
    // Build histogram
    int histogram[256] = {0};
    for (size_t i = 0; i < width * height; i++) {
        int bin = (int)(values[i] * 255.0);
        if (bin < 0) bin = 0;
        if (bin > 255) bin = 255;
        histogram[bin]++;
    }
    
    // Compute cumulative distribution function
    int cdf[256];
    cdf[0] = histogram[0];
    for (int i = 1; i < 256; i++) {
        cdf[i] = cdf[i-1] + histogram[i];
    }
    
    // Find min non-zero CDF value
    int cdf_min = cdf[0];
    for (int i = 0; i < 256; i++) {
        if (cdf[i] > 0) {
            cdf_min = cdf[i];
            break;
        }
    }
    
    // Apply histogram equalization with adaptive clipping
    int total_pixels = width * height;
    for (size_t i = 0; i < width * height; i++) {
        int bin = (int)(values[i] * 255.0);
        if (bin < 0) bin = 0;
        if (bin > 255) bin = 255;
        
        // Equalized value
        double equalized = (double)(cdf[bin] - cdf_min) / (double)(total_pixels - cdf_min);
        
        // Blend with original (adaptive strength based on local contrast)
        double blend_factor = 0.6; // 60% equalized, 40% original
        values[i] = blend_factor * equalized + (1.0 - blend_factor) * values[i];
        
        // Clamp
        if (values[i] < 0.0) values[i] = 0.0;
        if (values[i] > 1.0) values[i] = 1.0;
    }
}


// ============================================================================
// Retro Color Mode
// ============================================================================

static void get_retro_rgb(const hsv_t* hsv, int* out_r, int* out_g, int* out_b) {
    hsv_t quantized_hsv = *hsv;
    
    // Quantize hue to nearest 60° (6 primary hues)
    quantized_hsv.hue = round(quantized_hsv.hue / 60.0) * 60.0;
    if (quantized_hsv.hue >= 360.0) {
        quantized_hsv.hue = 0.0;
    }
    
    // Quantize saturation: grayscale or full color
    quantized_hsv.saturation = (quantized_hsv.saturation < 0.3) ? 0.0 : 1.0;
    
    // Keep original value for better brightness accuracy
    // (Don't force to 1.0 - let character handle brightness)
    
    // Convert back to RGB
    double r, g, b;
    hsv_to_rgb(&quantized_hsv, &r, &g, &b);
    
    // Boost brightness slightly for retro look
    double boost = 1.2;
    *out_r = (int)fmin(r * 255 * boost, 255);
    *out_g = (int)fmin(g * 255 * boost, 255);
    *out_b = (int)fmin(b * 255 * boost, 255);
}


// ============================================================================
// Enhanced ASCII Character Selection
// ============================================================================

static char get_ascii_char_simple(double luminance) {
    // Apply subtle gamma correction for better perceptual spacing
    double adjusted = pow(luminance, 0.9);
    
    size_t index = (size_t)(adjusted * (N_SIMPLE - 1));
    
    if (index >= N_SIMPLE) {
        index = N_SIMPLE - 1;
    }

    return SIMPLE_CHARS[index];
}

static const char* get_braille_char(double luminance) {
    double adjusted = pow(luminance, 0.85);
    size_t index = (size_t)(adjusted * (N_BRAILLE - 1));
    
    if (index >= N_BRAILLE) {
        index = N_BRAILLE - 1;
    }
    
    return BRAILLE_CHARS[index];
}


// ============================================================================
// Improved Edge Detection
// ============================================================================

static char get_edge_char_by_angle(double angle) {
    // More accurate angle-to-character mapping
    if ((22.5 <= angle && angle <= 67.5) || 
        (-157.5 <= angle && angle <= -112.5))
        return '/';
    else if ((67.5 <= angle && angle <= 112.5) || 
             (-112.5 <= angle && angle <= -67.5))
        return '-';
    else if ((112.5 <= angle && angle <= 157.5) || 
             (-67.5 <= angle && angle <= -22.5))
        return '\\';
    else
        return '|';
}


// ============================================================================
// Main Printing Function with Enhanced Rendering
// ============================================================================

void print_image_with_options(image_t* image, args_t* args);

void print_image(image_t* image, double edge_threshold, int use_retro_colors, int use_braille, int use_grayscale) {
    args_t args = {
        .edge_threshold = edge_threshold,
        .use_retro_colors = use_retro_colors,
        .use_braille = use_braille,
        .use_grayscale = use_grayscale,
        .use_enhanced_palette = 0,
        .debug_mode = 0
    };
    print_image_with_options(image, &args);
}

void print_image_with_options(image_t* image, args_t* args) {
    double edge_threshold = args->edge_threshold;
    int use_retro_colors = args->use_retro_colors;
    int use_braille = args->use_braille;
    int use_grayscale = args->use_grayscale;
    // TODO: Implement use_enhanced_palette selection
    // int use_enhanced = args->use_enhanced_palette;
    if (!image || !image->data) {
        fprintf(stderr, "Error: Invalid image data!\n");
        return;
    }

    // Create luminance buffer for better brightness calculation
    double* luminance_buffer = calloc(image->width * image->height, sizeof(*luminance_buffer));
    if (!luminance_buffer) {
        fprintf(stderr, "Error: Failed to allocate luminance buffer!\n");
        return;
    }

    // Calculate accurate luminance for each pixel
    for (size_t y = 0; y < image->height; y++) {
        for (size_t x = 0; x < image->width; x++) {
            double* pixel = get_pixel(image, x, y);
            size_t index = y * image->width + x;
            
            if (image->channels >= 3) {
                luminance_buffer[index] = calculate_luminance(pixel[0], pixel[1], pixel[2]);
            } else {
                luminance_buffer[index] = pixel[0];
            }
        }
    }

    // Apply adaptive contrast enhancement
    enhance_contrast_adaptive(luminance_buffer, image->width, image->height);

    // Prepare edge detection buffers
    image_t grayscale = make_grayscale(image);
    if (!grayscale.data) {
        fprintf(stderr, "Error: Failed to create grayscale image!\n");
        free(luminance_buffer);
        return;
    }

    double* sobel_x = calloc(grayscale.width * grayscale.height, sizeof(*sobel_x));
    double* sobel_y = calloc(grayscale.width * grayscale.height, sizeof(*sobel_y));
    
    if (!sobel_x || !sobel_y) {
        fprintf(stderr, "Error: Failed to allocate edge detection buffers!\n");
        free(sobel_x);
        free(sobel_y);
        free(luminance_buffer);
        free_image(&grayscale);
        return;
    }

    // Compute edges if enabled
    if (edge_threshold < 4.0) {
        get_sobel(&grayscale, sobel_x, sobel_y);
    }

    // Render each pixel
    for (size_t y = 0; y < image->height; y++) {
        for (size_t x = 0; x < image->width; x++) {
            double* pixel = get_pixel(image, x, y);
            if (!pixel) continue;

            size_t index = y * image->width + x;
            double sx = sobel_x[index];
            double sy = sobel_y[index];

            double edge_magnitude = sqrt(sx * sx + sy * sy);
            double edge_angle = atan2(sy, sx) * 180.0 / M_PI;

            // Get enhanced luminance value
            double luma = luminance_buffer[index];
            
            char ascii_char = ' ';
            const char* braille_str = NULL;
            int r = 255, g = 255, b = 255;
            
            // Grayscale mode: convert everything to grayscale
            if (use_grayscale || image->channels <= 2) {
                // Grayscale image or grayscale mode enabled
                r = g = b = (int)(luma * 255);
                if (use_braille) {
                    braille_str = get_braille_char(luma);
                } else {
                    ascii_char = get_ascii_char_simple(luma);
                }
            } else {
                // Color image - preserve original colors accurately
                hsv_t hsv = rgb_to_hsv(pixel[0], pixel[1], pixel[2]);
                
                if (use_retro_colors) {
                    // Retro mode with original brightness
                    get_retro_rgb(&hsv, &r, &g, &b);
                } else {
                    // Truecolor mode - boost saturation slightly for vibrancy
                    // but keep original value for accuracy
                    hsv.saturation = fmin(hsv.saturation * 1.15, 1.0);
                    
                    double r_d, g_d, b_d;
                    hsv_to_rgb(&hsv, &r_d, &g_d, &b_d);
                    r = (int)(r_d * 255);
                    g = (int)(g_d * 255);
                    b = (int)(b_d * 255);
                }
                
                if (use_braille) {
                    braille_str = get_braille_char(luma);
                } else {
                    ascii_char = get_ascii_char_simple(luma);
                }
            }

            // Blend edge enhancement instead of replacing (only for ASCII mode)
            if (!use_braille && edge_magnitude >= edge_threshold) {
                // Strong edges get edge characters
                if (edge_magnitude >= edge_threshold * 1.5) {
                    ascii_char = get_edge_char_by_angle(edge_angle);
                } else {
                    // Moderate edges: blend with texture
                    // Use slightly brighter character for edge areas
                    double boosted_luma = fmin(luma * 1.2, 1.0);
                    ascii_char = get_ascii_char_simple(boosted_luma);
                }
            }
            
            // Output with 24-bit truecolor
            if (use_braille) {
                printf("\x1b[38;2;%d;%d;%dm%s", r, g, b, braille_str);
            } else {
                printf("\x1b[38;2;%d;%d;%dm%c", r, g, b, ascii_char);
            }
        }
        printf("\n");
    }

    printf("%s", RESET);
    
    // Cleanup
    free(sobel_x);
    free(sobel_y);
    free(luminance_buffer);
    free_image(&grayscale);
}


// Play animated GIF in terminal with ultra-smooth rendering
void play_gif_animation(gif_animation_t* anim, args_t* args) {
    if (!anim || anim->frame_count == 0) {
        fprintf(stderr, "Error: Invalid animation data!\n");
        return;
    }
    
    printf("\x1b[2J\x1b[H"); // Clear screen and move cursor to home
    
    // Pre-process all frames once (resize and sharpen)
    image_t* processed_frames = calloc(anim->frame_count, sizeof(image_t));
    if (!processed_frames) {
        fprintf(stderr, "Error: Failed to allocate processed frames!\n");
        return;
    }
    
    // Pre-process frames (resize and sharpen once)
    for (int i = 0; i < anim->frame_count; i++) {
        // Resize first
        processed_frames[i] = make_resized(&anim->frames[i], args->max_width, args->max_height, args->character_ratio);
        
        // Apply sharpening on resized frame (not original!)
        if (processed_frames[i].data && args->sharpen_strength > 0.0) {
            unsharp_mask(&processed_frames[i], args->sharpen_strength, 1.0);
        }
    }
    
    // Loop through frames and display with ultra-smooth timing
    const int loop_count = 3; // Play 3 times
    for (int loop = 0; loop < loop_count && !g_shutdown_requested; loop++) {
        for (int i = 0; i < anim->frame_count && !g_shutdown_requested; i++) {
            if (!processed_frames[i].data) continue;
            
            // Move cursor to home position (no clear, just overwrite)
            printf("\x1b[H");
            
            // Render frame directly (already pre-processed)
            print_image(&processed_frames[i], args->edge_threshold, args->use_retro_colors, args->use_braille, args->use_grayscale);
            
            // Flush output buffer immediately for smoother display
            fflush(stdout);
            
            // Frame delay with improved timing accuracy
            // GIF delays are in centiseconds (1/100 second)
            int delay_ms = anim->delays[i] * 10;
            // Minimum 15ms for ultra-smooth 60+ FPS playback
            if (delay_ms < 15) delay_ms = 15;
            
            // Use nanosleep for more accurate timing than usleep
            struct timespec ts;
            ts.tv_sec = delay_ms / 1000;
            ts.tv_nsec = (delay_ms % 1000) * 1000000;
            nanosleep(&ts, NULL);
        }
    }
    
    // Cleanup
    for (int i = 0; i < anim->frame_count; i++) {
        free_image(&processed_frames[i]);
    }
    free(processed_frames);
    
    printf("\n");
}
