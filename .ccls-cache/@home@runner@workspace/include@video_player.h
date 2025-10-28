/*
 * ASCII-MEDIA - Video Player Module Header
 * 
 * Copyright (c) 2025 danko12
 * Adapted from ascii-video-player by SkwalExe
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

#ifndef ASCII_MEDIA_VIDEO_PLAYER_H
#define ASCII_MEDIA_VIDEO_PLAYER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    const char* file_path;
    int use_colors;
    int use_blocks;
    int use_grayscale;
    int show_status;
    double character_ratio;
    int max_width;
    int max_height;
} video_params_t;

int play_video_file(video_params_t* params);

void get_terminal_dimensions(int* width, int* height);

#ifdef __cplusplus
}
#endif

#endif // ASCII_MEDIA_VIDEO_PLAYER_H
