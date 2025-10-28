/*
 * ASCII Image Converter - Image Processing Library
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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"
#pragma GCC diagnostic pop

#include "../include/image.h"
#include <string.h>

// For GIF animation and timing
#include <unistd.h>
#include <sys/time.h>
#include <time.h>


image_t load_image(const char* file_path) {
    int width, height, channels;
    unsigned char* raw_data = stbi_load(file_path, &width, &height, &channels, 0);

    if (!raw_data) {
        fprintf(stderr, "Error: Failed to load image '%s': %s!\n", file_path, stbi_failure_reason());
        return (image_t) {0}; // Return empty image on failure
    }

    // Convert to [0., 1.]
    size_t total_size = (size_t) width * height * channels;
    double* data = calloc(total_size, sizeof(*data));
    if (!data) {
        fprintf(stderr, "Error: Failed to allocate memory for image data!\n");
        stbi_image_free(raw_data);
        return (image_t) {0}; // Return empty image on failure
    }

    for (size_t i = 0; i < total_size; i++) {
        data[i] = raw_data[i] / 255.0;
    }

    stbi_image_free(raw_data);

    return (image_t) {
        .width = (size_t) width,
        .height = (size_t) height,
        .channels = (size_t) channels,
        .data = data
    };
}


void free_image(image_t* image) {
    if (image && image->data) {
        free(image->data);
        image->data = NULL;
        image->width = image->height = image->channels = 0;
    }
}


// Gets pointer to pixel data at index (x, y)
double* get_pixel(image_t* image, size_t x, size_t y) {
    return &image->data[(y * image->width + x) * image->channels];
}


// Sets pixel channel values to those of new_pixel
void set_pixel(image_t* image, size_t x, size_t y, const double* new_pixel) {
    double* pixel = get_pixel(image, x, y);
    for (size_t c = 0; c < image->channels; c++) {
        pixel[c] = new_pixel[c];
    }
}


// Gets average pixel value in rectangular region; writes to `average`
void get_average(image_t* image, double* average, size_t x1, size_t x2, size_t y1, size_t y2) {
    // Set average to zero
    for (size_t c = 0; c < image->channels; c++) {
        average[c] = 0.0;
    }

    // Get total
    for (size_t y = y1; y < y2; y++) {
        for (size_t x = x1; x < x2; x++) {
            double* pixel = get_pixel(image, x, y);
            for (size_t c = 0; c < image->channels; c++) {
                average[c] += pixel[c];
            }
        }
    }

    // Divide by number of pixels in region
    double n_pixels = (double) (x2 - x1) * (y2 - y1);
    for (size_t c = 0; c < image->channels; c++) {
        average[c] /= n_pixels;
    }
}


image_t make_resized(image_t* original, size_t max_width, size_t max_height, double character_ratio) {
    size_t width, height;
    size_t channels = original->channels;

    // CRITICAL: Aspect ratio correction untuk mencegah gepeng
    // character_ratio = 2.0 karena karakter terminal tingginya 2x lebarnya
    // Rumus: aspect_corrected = (img_width / img_height) / character_ratio
    
    double img_width = (double)original->width;
    double img_height = (double)original->height;
    
    // Enforce minimum constraints FIRST (before scaling calculation)
    if (max_width < 10) max_width = 10;
    if (max_height < 8) max_height = 8;
    
    // Hitung aspect ratio asli gambar, SUDAH terkoreksi untuk terminal
    double img_aspect_corrected = img_width / (img_height * character_ratio);
    
    // Hitung aspect ratio dari area terminal yang tersedia
    double terminal_aspect = (double)max_width / (double)max_height;
    
    // Fit gambar ke terminal sambil MEMPERTAHANKAN aspect ratio
    if (img_aspect_corrected > terminal_aspect) {
        // Gambar lebih LEBAR relatif ke tingginya -> batasi oleh WIDTH
        width = max_width;
        height = (size_t)(((double)max_width / img_aspect_corrected) + 0.5);
        
        // Safety check: jika height melebihi, recompute KEDUA dimensi
        if (height > max_height) {
            height = max_height;
            width = (size_t)((double)max_height * img_aspect_corrected + 0.5);
        }
    } else {
        // Gambar lebih TINGGI relatif ke lebarnya -> batasi oleh HEIGHT
        height = max_height;
        width = (size_t)(((double)max_height * img_aspect_corrected) + 0.5);
        
        // Safety check: jika width melebihi, recompute KEDUA dimensi
        if (width > max_width) {
            width = max_width;
            height = (size_t)((double)max_width / img_aspect_corrected + 0.5);
        }
    }
    
    // NO POST-CALCULATION CLAMPS - they break aspect ratio!
    // Minimum size sudah dienforce di awal pada max_width/max_height
    
    // VERIFICATION: Check final aspect ratio matches original
    double final_aspect = ((double)width / (double)height) * character_ratio;
    double original_aspect = img_width / img_height;
    double deviation = fabs((final_aspect - original_aspect) / original_aspect);
    
    // If deviation > 3%, log warning
    if (deviation > 0.03) {
        fprintf(stderr, "⚠️  Aspect ratio deviation: %.1f%% (target: <3%%)\n", deviation * 100.0);
    }

    double* data = calloc(width * height * channels, sizeof(*data));
    if (!data) {
        fprintf(stderr, "Error: Failed to allocate memory for resized image!\n");
        return (image_t) {0};
    }

    // High-quality area averaging with improved sampling
    for (size_t j = 0; j < height; j++) {
        size_t y1 = (j * original->height) / height;
        size_t y2 = ((j + 1) * original->height) / height;
        if (y2 > original->height) y2 = original->height;
        if (y1 >= y2) y2 = y1 + 1;
        
        for (size_t i = 0; i < width; i++) {
            size_t x1 = (i * original->width) / width;
            size_t x2 = ((i + 1) * original->width) / width;
            if (x2 > original->width) x2 = original->width;
            if (x1 >= x2) x2 = x1 + 1;

            get_average(original, &data[(i + j * width) * channels], x1, x2, y1, y2);
        }
    }

    return (image_t) {
        .width = width,
        .height = height,
        .channels = channels,
        .data = data
    };
}


// Create grayscale version of image. Note: Assumes original is at least RGB.
image_t make_grayscale(image_t* original) {
    size_t width = original->width;
    size_t height = original->height;
    size_t channels = 1;

    double* data = calloc(width * height, sizeof(*data));
    if (!data) {
        fprintf(stderr, "Error: Failed to allocate memory for resized image!\n");
        return (image_t) {0};
    }

    image_t new = {
        .width = width,
        .height = height,
        .channels = channels,
        .data = data
    };

    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            double* pixel = get_pixel(original, x, y);

            // Luminance-weighted graycsale. Could be a callback...
            double grayscale = 0.2126 * pixel[0] + 0.7152 * pixel[1] + 0.0722 * pixel[2];

            set_pixel(&new, x, y, &grayscale);
        }
    }

    return new;
}


double calculate_convolution_value(image_t* image, double* kernel, size_t x, size_t y, size_t c) {
    double result = 0.0;

    for (int j = -1; j < 2; j++) {
        for (int i = -1; i < 2; i++) {
            size_t image_index = c + ((x + i) + (y + j) * image->width) * image->channels;
            size_t kernel_index = (i + 1) + (j + 1) * 3;

            result += kernel[kernel_index] * image->data[image_index];
        }
    }

    return result;
}


// Calculates convolution with 3x3 kernel. Ignores edges.
void get_convolution(image_t* image, double* kernel, double* out) {
    for (size_t y = 1; y < image->height - 1; y++) {
        for (size_t x = 1; x < image->width - 1; x++) {
            for (size_t c = 0; c < image->channels; c++) {
                size_t image_index = c + (x + y * image->width) * image->channels;
                out[image_index] = calculate_convolution_value(image, kernel, x, y, c);
            }
        }
    }
}


// Calculates sobel convolutions
void get_sobel(image_t* image, double* out_x, double* out_y) {
    double Gx[] = {-1., 0., 1., -2., 0., 2., -1., 0., 1};
    double Gy[] = {1., 2., 1., 0., 0., 0., -1., -2., -1};

    get_convolution(image, Gx, out_x);
    get_convolution(image, Gy, out_y);
}


// Sharpening filter - enhances edges and details
void sharpen_image(image_t* image, double strength) {
    if (!image || !image->data || strength <= 0.0) return;
    
    // Sharpening kernel
    double kernel[] = {
        0.0, -strength, 0.0,
        -strength, 1.0 + 4.0 * strength, -strength,
        0.0, -strength, 0.0
    };
    
    double* temp = calloc(image->width * image->height * image->channels, sizeof(*temp));
    if (!temp) {
        fprintf(stderr, "Error: Failed to allocate memory for sharpening!\n");
        return;
    }
    
    // Copy original data
    memcpy(temp, image->data, image->width * image->height * image->channels * sizeof(*temp));
    
    // Apply sharpening (skip edges)
    for (size_t y = 1; y < image->height - 1; y++) {
        for (size_t x = 1; x < image->width - 1; x++) {
            for (size_t c = 0; c < image->channels; c++) {
                double value = calculate_convolution_value(image, kernel, x, y, c);
                
                // Clamp value
                if (value < 0.0) value = 0.0;
                if (value > 1.0) value = 1.0;
                
                temp[c + (x + y * image->width) * image->channels] = value;
            }
        }
    }
    
    // Copy back
    memcpy(image->data, temp, image->width * image->height * image->channels * sizeof(*temp));
    free(temp);
}


// Unsharp mask - professional sharpening technique
void unsharp_mask(image_t* image, double amount, double radius) {
    if (!image || !image->data || amount <= 0.0) return;
    
    // Suppress unused parameter warning (radius currently fixed at 1.0)
    (void)radius;
    
    // Simple gaussian blur kernel (approximation for radius ~1.0)
    double blur_kernel[] = {
        1.0/16, 2.0/16, 1.0/16,
        2.0/16, 4.0/16, 2.0/16,
        1.0/16, 2.0/16, 1.0/16
    };
    
    double* blurred = calloc(image->width * image->height * image->channels, sizeof(*blurred));
    if (!blurred) {
        fprintf(stderr, "Error: Failed to allocate memory for unsharp mask!\n");
        return;
    }
    
    // Apply gaussian blur
    for (size_t y = 1; y < image->height - 1; y++) {
        for (size_t x = 1; x < image->width - 1; x++) {
            for (size_t c = 0; c < image->channels; c++) {
                blurred[c + (x + y * image->width) * image->channels] = 
                    calculate_convolution_value(image, blur_kernel, x, y, c);
            }
        }
    }
    
    // Unsharp mask formula: sharpened = original + amount * (original - blurred)
    for (size_t i = 0; i < image->width * image->height * image->channels; i++) {
        double sharp = image->data[i] + amount * (image->data[i] - blurred[i]);
        
        // Clamp
        if (sharp < 0.0) sharp = 0.0;
        if (sharp > 1.0) sharp = 1.0;
        
        image->data[i] = sharp;
    }
    
    free(blurred);
}


// Check if file is a GIF
int is_gif_file(const char* file_path) {
    FILE* f = fopen(file_path, "rb");
    if (!f) return 0;
    
    unsigned char header[6];
    size_t read = fread(header, 1, 6, f);
    fclose(f);
    
    if (read != 6) return 0;
    
    // Check for GIF87a or GIF89a signature
    return (header[0] == 'G' && header[1] == 'I' && header[2] == 'F' &&
            header[3] == '8' && (header[4] == '7' || header[4] == '9') &&
            header[5] == 'a');
}


// Load animated GIF using stb_image GIF API
gif_animation_t load_gif_animation(const char* file_path) {
    gif_animation_t anim = {0};
    
    // Read entire file into memory
    FILE* f = fopen(file_path, "rb");
    if (!f) {
        fprintf(stderr, "Error: Cannot open file '%s'\n", file_path);
        return anim;
    }
    
    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    unsigned char* file_data = malloc(file_size);
    if (!file_data) {
        fprintf(stderr, "Error: Failed to allocate memory for GIF data\n");
        fclose(f);
        return anim;
    }
    
    fread(file_data, 1, file_size, f);
    fclose(f);
    
    // Load GIF with all frames using stbi_load_gif_from_memory
    int* delays_centiseconds = NULL;
    int width = 0, height = 0, frame_count = 0, channels = 0;
    
    // Load all GIF frames at once
    stbi_uc* raw_frames = stbi_load_gif_from_memory(
        file_data, file_size,
        &delays_centiseconds,
        &width, &height, &frame_count, &channels,
        0  // 0 = use image's channel count
    );
    
    free(file_data);
    
    if (!raw_frames || frame_count == 0) {
        fprintf(stderr, "Error: Failed to load GIF animation: %s\n", stbi_failure_reason());
        if (delays_centiseconds) free(delays_centiseconds);
        return anim;
    }
    
    printf("Loaded GIF: %d frames, %dx%d, %d channels\n", frame_count, width, height, channels);
    
    // Allocate arrays for frames
    anim.frame_count = frame_count;
    anim.frames = calloc(frame_count, sizeof(image_t));
    anim.delays = calloc(frame_count, sizeof(int));
    
    if (!anim.frames || !anim.delays) {
        fprintf(stderr, "Error: Failed to allocate memory for frames\n");
        stbi_image_free(raw_frames);
        free(delays_centiseconds);
        return anim;
    }
    
    // Copy delays
    for (int i = 0; i < frame_count; i++) {
        anim.delays[i] = delays_centiseconds[i];
    }
    free(delays_centiseconds);
    
    // Convert each frame from raw bytes to our image_t format
    size_t frame_size = (size_t)width * height * channels;
    for (int i = 0; i < frame_count; i++) {
        double* data = calloc(frame_size, sizeof(*data));
        if (!data) {
            fprintf(stderr, "Error: Failed to allocate frame %d\n", i);
            continue;
        }
        
        // Convert frame data from 0-255 to 0.0-1.0
        stbi_uc* frame_start = raw_frames + (i * frame_size);
        for (size_t j = 0; j < frame_size; j++) {
            data[j] = frame_start[j] / 255.0;
        }
        
        anim.frames[i].width = width;
        anim.frames[i].height = height;
        anim.frames[i].channels = channels;
        anim.frames[i].data = data;
    }
    
    stbi_image_free(raw_frames);
    
    return anim;
}


// Free GIF animation
void free_gif_animation(gif_animation_t* anim) {
    if (!anim) return;
    
    for (int i = 0; i < anim->frame_count; i++) {
        free_image(&anim->frames[i]);
    }
    
    free(anim->frames);
    free(anim->delays);
    
    anim->frames = NULL;
    anim->delays = NULL;
    anim->frame_count = 0;
}
