/*
 * ASCII-MEDIA - Video Player Main Entry
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
 */

#include <iostream>
#include <cstring>
#include "../include/video_player.h"

using namespace std;

void print_help() {
    cout << "\n\033[97mASCII-MEDIA Video Player v3.0.0\033[0m\n";
    cout << "\033[95m━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\033[0m\n";
    cout << "\033[97mUsage:\033[0m \033[95mascii-video\033[0m \033[93m<video-file> [OPTIONS]\033[0m\n\n";
    cout << "\033[95mOptions:\033[0m\n";
    cout << "  \033[93m-c, --colors\033[0m          Enable colored ASCII output\n";
    cout << "  \033[93m-b, --blocks\033[0m          Use colored blocks (requires -c)\n";
    cout << "  \033[93m-g, --grayscale\033[0m       Black and white mode\n";
    cout << "  \033[93m-s, --status\033[0m          Show playback status\n";
    cout << "  \033[93m-w, --width <N>\033[0m       Set width (default: auto-detect)\n";
    cout << "  \033[93m-h, --height <N>\033[0m      Set height (default: auto-detect)\n";
    cout << "  \033[93m--help\033[0m                Show this help\n\n";
    cout << "\033[95mExamples:\033[0m\n";
    cout << "  \033[97mascii-video video.mp4\033[0m\n";
    cout << "  \033[97mascii-video video.mp4 -c\033[0m\n";
    cout << "  \033[97mascii-video video.mp4 -c -b\033[0m\n";
    cout << "\033[95m━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\033[0m\n\n";
}

int main(int argc, char* argv[]) {
    video_params_t params = {0};
    params.character_ratio = 2.0;
    params.max_width = 0;
    params.max_height = 0;
    params.use_colors = 0;
    params.use_blocks = 0;
    params.use_grayscale = 0;
    params.show_status = 0;
    
    if (argc < 2) {
        cerr << "\033[91mError: No video file specified\033[0m\n";
        print_help();
        return 1;
    }
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            print_help();
            return 0;
        } else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--colors") == 0) {
            params.use_colors = 1;
        } else if (strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--blocks") == 0) {
            params.use_blocks = 1;
            params.use_colors = 1;
        } else if (strcmp(argv[i], "-g") == 0 || strcmp(argv[i], "--grayscale") == 0) {
            params.use_grayscale = 1;
        } else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--status") == 0) {
            params.show_status = 1;
        } else if (strcmp(argv[i], "-w") == 0 || strcmp(argv[i], "--width") == 0) {
            if (i + 1 < argc) {
                params.max_width = atoi(argv[++i]);
            }
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--height") == 0) {
            if (i + 1 < argc) {
                params.max_height = atoi(argv[++i]);
            }
        } else if (params.file_path == nullptr) {
            params.file_path = argv[i];
        } else {
            cerr << "\033[91mError: Unknown argument: " << argv[i] << "\033[0m\n";
            return 1;
        }
    }
    
    if (!params.file_path) {
        cerr << "\033[91mError: No video file specified\033[0m\n";
        print_help();
        return 1;
    }
    
    return play_video_file(&params);
}
