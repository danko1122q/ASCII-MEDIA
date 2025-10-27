#!/bin/bash
# ASCII Image Converter - Build Script for Unix-like systems (Linux, macOS)
# 
# Copyright (c) 2025 danko12
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

set -e

echo "==================================="
echo "ASCII View - Build Script"
echo "==================================="

# Check if CMake is available
if command -v cmake &> /dev/null; then
    echo "Using CMake build system..."
    mkdir -p build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Release ..
    cmake --build .
    echo ""
    echo "Build complete! Executable: build/ascii-view"
    echo "To install system-wide, run: sudo cmake --install ."
# Fallback to Make
elif command -v make &> /dev/null; then
    echo "Using Make build system..."
    make release
    echo ""
    echo "Build complete! Executable: ./ascii-view"
else
    echo "Error: Neither CMake nor Make found!"
    echo "Please install CMake or Make to build this project."
    exit 1
fi

echo ""
echo "Build successful!"
