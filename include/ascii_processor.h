/*
 * ASCII-MEDIA - C++ Processor Module Header
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

#ifndef ASCII_PROCESSOR_H
#define ASCII_PROCESSOR_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// Error codes
#define ASCII_OK 0
#define ASCII_INVALID_ARG 1
#define ASCII_DECODE_ERROR 2
#define ASCII_OOM 3
#define ASCII_UNSUPPORTED_FORMAT 4

/**
 * Process frame with optimized C++ algorithms
 * @param rgba Input RGBA frame data
 * @param width Frame width in pixels
 * @param height Frame height in pixels
 * @param target_width Target ASCII width
 * @param target_height Target ASCII height
 * @param out_processed Pointer to output processed data (caller must free)
 * @return Error code (ASCII_OK on success)
 */
int process_frame_optimized(const uint8_t* rgba, int width, int height,
                           int target_width, int target_height,
                           uint8_t** out_processed);

/**
 * Calculate perceptually accurate luminance using ITU-R BT.709
 * @param rgb Input RGB values (0.0-1.0 range)
 * @param out_luminance Output luminance value
 */
void calculate_luminance_accurate(const double* rgb, double* out_luminance);

/**
 * Advanced resize with better quality than simple area averaging
 * @param input Input image data
 * @param in_width Input width
 * @param in_height Input height
 * @param out_width Output width
 * @param out_height Output height
 * @param channels Number of color channels
 * @param output Output buffer (must be pre-allocated)
 * @return Error code
 */
int advanced_resize(const double* input, size_t in_width, size_t in_height,
                   size_t out_width, size_t out_height, size_t channels,
                   double* output);

/**
 * Free memory allocated by processor functions
 * @param ptr Pointer to free
 */
void free_processed_data(void* ptr);

#ifdef __cplusplus
}
#endif

#endif // ASCII_PROCESSOR_H
