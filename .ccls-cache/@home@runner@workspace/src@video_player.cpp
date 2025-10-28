/*
 * ASCII-MEDIA - Video Player Module Implementation
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

#include "../include/video_player.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cstring>
#include <sys/ioctl.h>
#include <unistd.h>
#include <thread>
#include <chrono>

using namespace std;
using namespace cv;

static const char ASCII_CHARS[] = " .'`^\",:;Il!i><~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";

extern "C" {

void get_terminal_dimensions(int* width, int* height) {
    struct winsize w;
    ioctl(fileno(stdout), TIOCGWINSZ, &w);
    *width = (int)(w.ws_col);
    *height = (int)(w.ws_row - 2);
}

static void fit_frame_to_terminal(Mat& frame, int max_width, int max_height, double character_ratio) {
    if (frame.empty()) return;
    
    max_width -= 1;
    max_height -= 1;
    
    double img_width = (double)frame.cols;
    double img_height = (double)frame.rows;
    
    double target_aspect = img_width / (img_height * character_ratio);
    double max_aspect = (double)max_width / (double)max_height;
    
    int new_width, new_height;
    
    if (target_aspect > max_aspect) {
        new_width = max_width;
        new_height = (int)((double)max_width / target_aspect + 0.5);
        if (new_height > max_height) new_height = max_height;
    } else {
        new_height = max_height;
        new_width = (int)((double)max_height * target_aspect + 0.5);
        if (new_width > max_width) new_width = max_width;
    }
    
    if (new_width < 10) new_width = 10;
    if (new_height < 8) new_height = 8;
    
    resize(frame, frame, Size(new_width, new_height), 0, 0, INTER_AREA);
}

static void display_frame_grayscale(const Mat& frame) {
    static int chars_array_len = sizeof(ASCII_CHARS) - 1;
    
    for (int y = 0; y < frame.rows; y++) {
        for (int x = 0; x < frame.cols; x++) {
            int pixel_value = frame.at<uchar>(y, x);
            int char_index = (chars_array_len * pixel_value) / 255;
            if (char_index >= chars_array_len) char_index = chars_array_len - 1;
            cout << ASCII_CHARS[char_index];
        }
        cout << "\n";
    }
    cout << flush;
}

static void display_frame_color(const Mat& frame, int use_blocks) {
    static int chars_array_len = sizeof(ASCII_CHARS) - 1;
    
    for (int y = 0; y < frame.rows; y++) {
        for (int x = 0; x < frame.cols; x++) {
            Vec3b pixel = frame.at<Vec3b>(y, x);
            
            if (use_blocks) {
                cout << "\x1b[48;2;"
                     << (int)pixel[2] << ";" << (int)pixel[1] << ";" << (int)pixel[0]
                     << "m  \x1b[0m";
            } else {
                int gray_value = (pixel[0] + pixel[1] + pixel[2]) / 3;
                int char_index = (chars_array_len * gray_value) / 255;
                if (char_index >= chars_array_len) char_index = chars_array_len - 1;
                
                cout << "\x1b[38;2;"
                     << (int)pixel[2] << ";" << (int)pixel[1] << ";" << (int)pixel[0]
                     << "m" << ASCII_CHARS[char_index] << "\x1b[0m";
            }
        }
        cout << "\n";
    }
    cout << flush;
}

int play_video_file(video_params_t* params) {
    if (!params || !params->file_path) {
        cerr << "Error: Invalid parameters" << endl;
        return 1;
    }
    
    VideoCapture cap(params->file_path);
    
    if (!cap.isOpened()) {
        cerr << "Error: Cannot open video file: " << params->file_path << endl;
        return 1;
    }
    
    double frame_count = cap.get(CAP_PROP_FRAME_COUNT);
    double fps = cap.get(CAP_PROP_FPS);
    double duration = frame_count / fps;
    int frame_duration_ms = (int)((duration * 1000.0) / frame_count);
    
    int width = params->max_width;
    int height = params->max_height;
    
    if (width <= 0 || height <= 0) {
        get_terminal_dimensions(&width, &height);
    }
    
    cout << "\n\033[95m╔══════════════════════════════════════════════════════════════╗\033[0m\n";
    cout << "\033[95m║\033[0m           \033[97mASCII-MEDIA Video Player v3.0.0\033[0m              \033[95m║\033[0m\n";
    cout << "\033[95m╚══════════════════════════════════════════════════════════════╝\033[0m\n";
    cout << "\033[95mFile:\033[0m         \033[93m" << params->file_path << "\033[0m\n";
    cout << "\033[95mDuration:\033[0m     \033[93m" << duration << " seconds\033[0m\n";
    cout << "\033[95mFPS:\033[0m          \033[93m" << fps << "\033[0m\n";
    cout << "\033[95mFrames:\033[0m       \033[93m" << frame_count << "\033[0m\n";
    cout << "\033[95mTerminal:\033[0m     \033[93m" << width << "x" << height << "\033[0m\n";
    cout << "\033[95mResolution:\033[0m   \033[93m" << cap.get(CAP_PROP_FRAME_WIDTH) << "x" 
         << cap.get(CAP_PROP_FRAME_HEIGHT) << "\033[0m\n";
    cout << "\n\033[93mPress Enter to start...\033[0m";
    cin.get();
    
    Mat frame;
    
    for (int i = 0; i < frame_count; i++) {
        auto start_time = chrono::high_resolution_clock::now();
        
        if (!cap.read(frame)) break;
        if (frame.empty()) break;
        
        if (params->use_grayscale || !params->use_colors) {
            cvtColor(frame, frame, COLOR_BGR2GRAY);
        }
        
        fit_frame_to_terminal(frame, width, height, params->character_ratio);
        
        cout << "\033[2J\033[H";
        
        if (params->show_status) {
            cout << "\033[91m" << i << "/" << (int)frame_count << " (" 
                 << (int)((i * 100) / frame_count) << "%)\033[0m\n";
        }
        
        if (params->use_grayscale || !params->use_colors) {
            display_frame_grayscale(frame);
        } else {
            display_frame_color(frame, params->use_blocks);
        }
        
        auto end_time = chrono::high_resolution_clock::now();
        auto elapsed = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
        int sleep_time = frame_duration_ms - (int)elapsed;
        
        if (sleep_time > 0) {
            this_thread::sleep_for(chrono::milliseconds(sleep_time));
        }
    }
    
    cout << "\033[2J\033[H";
    cout << "\033[93m✅ Video playback complete!\033[0m\n";
    
    return 0;
}

}
