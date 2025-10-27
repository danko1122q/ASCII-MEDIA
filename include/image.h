/*
 * ASCII Image Converter - Image Processing Header
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

#ifndef ASCIIVIEW_IMAGE_H
#define ASCIIVIEW_IMAGE_H

#include <stdlib.h>
#include <stdio.h>

typedef struct {
    size_t width;
    size_t height;
    size_t channels;
    double* data;
} image_t;

// Image loading and management
image_t load_image(const char* file_path);
void free_image(image_t* image);

// Image transformations
image_t make_resized(image_t* original, size_t max_width, size_t max_height, double character_ratio);
image_t make_grayscale(image_t* original);

// Pixel operations
double* get_pixel(image_t* image, size_t x, size_t y);
void set_pixel(image_t* image, size_t x, size_t y, const double* new_pixel);

// Edge detection
void get_convolution(image_t* image, double* kernel, double* out);
void get_sobel(image_t* image, double* out_x, double* out_y);

// Image enhancement
void sharpen_image(image_t* image, double strength);
void unsharp_mask(image_t* image, double amount, double radius);

// GIF animation support
typedef struct {
    int frame_count;
    int* delays;
    image_t* frames;
} gif_animation_t;

int is_gif_file(const char* file_path);
gif_animation_t load_gif_animation(const char* file_path);
void free_gif_animation(gif_animation_t* anim);

#endif
