# ASCII-MEDIA - CMake Build Configuration (Hybrid C/C++)
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

cmake_minimum_required(VERSION 3.10)
project(ascii-media VERSION 2.3.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler warnings (GCC/Clang for Linux/macOS)
add_compile_options(-Wall -Wextra -Wpedantic)

# Source files
set(C_SOURCES
    src/main.c
    src/argparse.c
    src/image.c
    src/print_image.c
)

set(CXX_SOURCES
    src/ascii_processor.cpp
)

# Include directories
include_directories(include)

# Create executable (C/C++ hybrid)
add_executable(ascii ${C_SOURCES} ${CXX_SOURCES})

# Link libraries (Linux/macOS)
target_link_libraries(ascii m stdc++)

# Add _GNU_SOURCE for POSIX systems
if(UNIX)
    add_definitions(-D_GNU_SOURCE)
endif()

# Installation
install(TARGETS ascii DESTINATION bin)

# Build type defaults
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Optimization flags for Release build (Linux/macOS)
if(CMAKE_BUILD_TYPE MATCHES Release)
    add_compile_options(-O3)
    if(NOT APPLE)
        # LTO and native arch optimizations for Linux
        add_compile_options(-flto -march=native)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -flto")
    endif()
endif()
