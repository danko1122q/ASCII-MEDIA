/*
 * ASCII-MEDIA - C++ Processor Module Implementation
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

#include "../include/ascii_processor.h"
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <vector>

// Namespace for internal C++ utilities
namespace ascii {

// sRGB gamma correction
inline double srgb_to_linear(double channel) {
    if (channel <= 0.04045) {
        return channel / 12.92;
    } else {
        return std::pow((channel + 0.055) / 1.055, 2.4);
    }
}

inline double linear_to_srgb(double channel) {
    if (channel <= 0.0031308) {
        return 12.92 * channel;
    } else {
        return 1.055 * std::pow(channel, 1.0 / 2.4) - 0.055;
    }
}

// Clamp value to range [0, 1]
inline double clamp(double value) {
    return std::max(0.0, std::min(1.0, value));
}

// Bicubic interpolation kernel
inline double cubic_kernel(double x) {
    x = std::abs(x);
    if (x <= 1.0) {
        return 1.5 * x * x * x - 2.5 * x * x + 1.0;
    } else if (x < 2.0) {
        return -0.5 * x * x * x + 2.5 * x * x - 4.0 * x + 2.0;
    }
    return 0.0;
}

// Bilinear interpolation for better quality
double bilinear_sample(const double* image, size_t width, size_t height,
                      size_t channels, double x, double y, size_t channel) {
    // Clamp coordinates
    x = std::max(0.0, std::min(x, static_cast<double>(width - 1)));
    y = std::max(0.0, std::min(y, static_cast<double>(height - 1)));
    
    size_t x0 = static_cast<size_t>(x);
    size_t y0 = static_cast<size_t>(y);
    size_t x1 = std::min(x0 + 1, width - 1);
    size_t y1 = std::min(y0 + 1, height - 1);
    
    double dx = x - x0;
    double dy = y - y0;
    
    double v00 = image[(y0 * width + x0) * channels + channel];
    double v10 = image[(y0 * width + x1) * channels + channel];
    double v01 = image[(y1 * width + x0) * channels + channel];
    double v11 = image[(y1 * width + x1) * channels + channel];
    
    double v0 = v00 * (1.0 - dx) + v10 * dx;
    double v1 = v01 * (1.0 - dx) + v11 * dx;
    
    return v0 * (1.0 - dy) + v1 * dy;
}

} // namespace ascii

// C API implementations

extern "C" {

void calculate_luminance_accurate(const double* rgb, double* out_luminance) {
    if (!rgb || !out_luminance) return;
    
    // Convert to linear RGB
    double r_lin = ascii::srgb_to_linear(rgb[0]);
    double g_lin = ascii::srgb_to_linear(rgb[1]);
    double b_lin = ascii::srgb_to_linear(rgb[2]);
    
    // Apply ITU-R BT.709 weights
    double luminance_lin = 0.2126 * r_lin + 0.7152 * g_lin + 0.0722 * b_lin;
    
    // Convert back to sRGB for display
    *out_luminance = ascii::linear_to_srgb(luminance_lin);
}

int advanced_resize(const double* input, size_t in_width, size_t in_height,
                   size_t out_width, size_t out_height, size_t channels,
                   double* output) {
    if (!input || !output || in_width == 0 || in_height == 0 ||
        out_width == 0 || out_height == 0 || channels == 0) {
        return ASCII_INVALID_ARG;
    }
    
    double x_ratio = static_cast<double>(in_width) / static_cast<double>(out_width);
    double y_ratio = static_cast<double>(in_height) / static_cast<double>(out_height);
    
    // Use bilinear interpolation for better quality
    for (size_t y = 0; y < out_height; y++) {
        for (size_t x = 0; x < out_width; x++) {
            double src_x = (x + 0.5) * x_ratio - 0.5;
            double src_y = (y + 0.5) * y_ratio - 0.5;
            
            for (size_t c = 0; c < channels; c++) {
                double value = ascii::bilinear_sample(input, in_width, in_height,
                                                     channels, src_x, src_y, c);
                output[(y * out_width + x) * channels + c] = ascii::clamp(value);
            }
        }
    }
    
    return ASCII_OK;
}

int process_frame_optimized(const uint8_t* rgba, int width, int height,
                           int target_width, int target_height,
                           uint8_t** out_processed) {
    if (!rgba || !out_processed || width <= 0 || height <= 0 ||
        target_width <= 0 || target_height <= 0) {
        return ASCII_INVALID_ARG;
    }
    
    size_t input_size = static_cast<size_t>(width * height * 4);
    size_t output_size = static_cast<size_t>(target_width * target_height * 4);
    
    // Allocate output buffer
    uint8_t* output = static_cast<uint8_t*>(malloc(output_size));
    if (!output) {
        return ASCII_OOM;
    }
    
    // Convert input to double precision for processing
    std::vector<double> input_double(input_size);
    for (size_t i = 0; i < input_size; i++) {
        input_double[i] = rgba[i] / 255.0;
    }
    
    // Resize with advanced interpolation
    std::vector<double> output_double(output_size);
    int result = advanced_resize(input_double.data(),
                                width, height,
                                target_width, target_height,
                                4, output_double.data());
    
    if (result != ASCII_OK) {
        free(output);
        return result;
    }
    
    // Convert back to uint8_t
    for (size_t i = 0; i < output_size; i++) {
        output[i] = static_cast<uint8_t>(ascii::clamp(output_double[i]) * 255.0);
    }
    
    *out_processed = output;
    return ASCII_OK;
}

void free_processed_data(void* ptr) {
    if (ptr) {
        free(ptr);
    }
}

} // extern "C"
