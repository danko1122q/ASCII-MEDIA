# ASCII-MEDIA - CMake Build Configuration (Hybrid C/C++)
# 
# Copyright (c) 2025 danko12

cmake_minimum_required(VERSION 3.10)
project(ascii-media VERSION 3.0.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler warnings
add_compile_options(-Wall -Wextra -Wpedantic)

# Source files for image/GIF processor
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

# Build Image/GIF processor
add_executable(ascii ${C_SOURCES} ${CXX_SOURCES})
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

# Optimization flags for Release build
if(CMAKE_BUILD_TYPE MATCHES Release)
    add_compile_options(-O3)
    if(NOT APPLE)
        add_compile_options(-flto -march=native)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -flto")
    endif()
endif()
