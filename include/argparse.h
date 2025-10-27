/*
 * ASCII Image Converter - Argument Parser Header
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

#ifndef ASCIIVIEW_ARGPARSE_H
#define ASCIIVIEW_ARGPARSE_H

#include <stdlib.h>
#include <signal.h>

typedef struct {
    char* file_path;
    size_t max_width;
    size_t max_height;
    double character_ratio;
    double edge_threshold;
    int use_retro_colors;
    double sharpen_strength;
    int use_braille;
    int animate_gif;
    int dimension_preset;
    int use_grayscale;
    int debug_mode;
    int use_enhanced_palette;
} args_t;

args_t parse_args(int argc, char* argv[]);
void setup_signal_handlers(void);
int terminal_was_resized(void);

extern volatile sig_atomic_t g_shutdown_requested;

#endif
