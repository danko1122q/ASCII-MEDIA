# ASCII-MEDIA v3.0.0 - Technical Documentation

**Linux & macOS Only** | Windows support removed

## Table of Contents
1. [Architecture Overview](#architecture-overview)
2. [Image Processing Pipeline](#image-processing-pipeline)
3. [Luminance Calculation (BT.601)](#luminance-calculation-bt601)
4. [Smart Aspect Ratio Lock](#smart-aspect-ratio-lock)
5. [Signal Handling (SIGWINCH & SIGINT)](#signal-handling)
6. [Enhanced Character Palettes](#enhanced-character-palettes)
7. [GIF Animation System (Double Buffering)](#gif-animation-system)
8. [Video/Webcam Integration (tplay)](#video-webcam-integration)
9. [Performance Optimizations](#performance-optimizations)
10. [Build System](#build-system)

---

## Architecture Overview

### System Design

The converter follows a modular pipeline architecture:

```
Input → Load → Process → Render → Output
  ↓       ↓        ↓         ↓        ↓
File  stb_image  Enhance  Character Terminal
Path            Resize   Mapping   (ANSI)
                Sharpen
```

### Module Structure

```
ascii-image-converter/
├── src/
│   ├── main.c          # Entry point and flow control
│   ├── argparse.c      # Command-line argument parsing
│   ├── image.c         # Image loading and processing
│   └── print_image.c   # Rendering and output
├── include/
│   ├── argparse.h      # CLI interface definitions
│   ├── image.h         # Image structures and functions
│   ├── print_image.h   # Rendering functions
│   └── stb_image.h     # Image loading library
```

---

## Image Processing Pipeline

### 1. Image Loading

**Library**: stb_image.h v2.28 (Sean Barrett)

**Supported Formats**:
- JPEG/JPG (baseline, progressive)
- PNG (all bit depths, interlaced)
- BMP, GIF (static and animated)
- TGA, PSD, HDR, PIC, PNM

**Loading Process**:
```c
// Static images
stbi_uc* data = stbi_load(filename, &width, &height, &channels, 0);

// Animated GIFs
stbi_uc* frames = stbi_load_gif_from_memory(
    buffer, size, &delays, &width, &height, &frame_count, &channels, 0
);
```

**Data Normalization**:
```c
// Convert from 0-255 to 0.0-1.0 for processing
for (size_t i = 0; i < total_pixels; i++) {
    image->data[i] = raw_data[i] / 255.0;
}
```

### 2. Unsharp Mask Sharpening

**Algorithm**: Professional unsharp masking

**Formula**:
```
sharpened = original + amount × (original - blurred)
```

**Implementation**:
```c
void unsharp_mask(image_t* image, double amount, double radius) {
    // 3x3 Gaussian blur kernel
    double kernel[9] = {
        1/16.0, 2/16.0, 1/16.0,
        2/16.0, 4/16.0, 2/16.0,
        1/16.0, 2/16.0, 1/16.0
    };
    
    // Apply convolution and enhance
    for each pixel:
        blurred = convolve(image, kernel, x, y)
        detail = original - blurred
        sharpened = clamp(original + (amount × detail), 0.0, 1.0)
}
```

**Parameters**:
- **0.5-1.0**: Subtle enhancement (portraits)
- **1.0-1.5**: Standard sharpening (general use)
- **1.5-2.0**: Aggressive sharpening (architecture)

### 3. Image Resizing

**Algorithm**: Area averaging with aspect ratio preservation

**Process**:
```c
// Calculate target dimensions accounting for character aspect ratio
target_width = min(max_width, image_width / character_ratio)
target_height = min(max_height, image_height)

// Maintain aspect ratio
scale = min(target_width / image_width, target_height / image_height)

final_width = image_width × scale / character_ratio
final_height = image_height × scale
```

**Area Averaging**: Samples multiple source pixels per output pixel to prevent aliasing and preserve brightness.

### 4. Luminance Calculation

**Standard**: ITU-R BT.709 (HDTV standard)

**Formula**:
```
Y = 0.2126 × R + 0.7152 × G + 0.0722 × B
```

**Gamma-Aware Conversion**:
```c
// sRGB to linear RGB
double linearize(double c) {
    if (c <= 0.04045)
        return c / 12.92;
    return pow((c + 0.055) / 1.055, 2.4);
}

// Calculate luminance in linear space
R_linear = linearize(R)
G_linear = linearize(G)
B_linear = linearize(B)
Y = 0.2126 × R_linear + 0.7152 × G_linear + 0.0722 × B_linear
```

### 5. Adaptive Contrast Enhancement

**Algorithm**: Histogram Equalization (CLAHE variant)

**Implementation**:
```c
void enhance_contrast_adaptive(double* luminance, int width, int height) {
    // Compute histogram
    int histogram[256] = {0};
    for each pixel:
        bin = luminance[pixel] × 255
        histogram[bin]++
    
    // Compute cumulative distribution function
    int cdf[256]
    cdf[0] = histogram[0]
    for i = 1 to 255:
        cdf[i] = cdf[i-1] + histogram[i]
    
    // Apply equalization with blending
    for each pixel:
        enhanced = (cdf[bin] - cdf_min) / (total_pixels - cdf_min)
        // 60% enhanced, 40% original
        luminance[pixel] = 0.6 × enhanced + 0.4 × original
}
```

### 6. Edge Detection

**Algorithm**: Sobel operator

**Kernels**:
```
Gx (horizontal):          Gy (vertical):
[-1  0  +1]              [-1  -2  -1]
[-2  0  +2]              [ 0   0   0]
[-1  0  +1]              [+1  +2  +1]
```

**Computation**:
```c
Gx = convolve(image, sobel_x, x, y)
Gy = convolve(image, sobel_y, x, y)
magnitude = sqrt(Gx² + Gy²)
angle = atan2(Gy, Gx) × 180 / π
```

**Directional Mapping**:
```
-22.5° to 22.5°  → | (vertical)
22.5° to 67.5°   → / (diagonal)
67.5° to 112.5°  → - (horizontal)
112.5° to 157.5° → \ (diagonal)
```

---

## Rendering Systems

### ASCII Mode

**Character Ramp**: `` .:-=+*#%@`` (11 levels)

**Mapping**:
```c
const char* ASCII_RAMP = " .:-=+*#%@";
char get_ascii_char(double luminance) {
    int index = (int)(luminance × 10.0);
    return ASCII_RAMP[clamp(index, 0, 10)];
}
```

### Braille Mode

**Unicode Range**: U+2800 to U+28FF

**Character Set**: `⠀⠁⠃⠇⠏⠟⠿⣿` (8 levels)

**Advantages**:
- Doubles effective resolution
- Smoother gradients
- Superior detail rendering

**Mapping**:
```c
const char* BRAILLE_CHARS[] = {
    "⠀", "⠁", "⠃", "⠇", "⠏", "⠟", "⠿", "⣿"
};
const char* get_braille_char(double luminance) {
    int index = (int)(luminance × 7.0);
    return BRAILLE_CHARS[clamp(index, 0, 7)];
}
```

### Grayscale Mode

**Conversion**: Disables color output, uses only luminance values

**Implementation**:
```c
if (use_grayscale) {
    // Output character without color codes
    printf("%c", character);
} else {
    // Output with 24-bit color
    printf("\x1b[38;2;%d;%d;%dm%c", r, g, b, character);
}
```

**Benefits**:
- Artistic black-and-white effects
- Better terminal compatibility
- Reduced visual complexity
- Works with all rendering modes

### Color Processing

**True Color Mode (Default)**:
```c
printf("\x1b[38;2;%d;%d;%dm%c", r, g, b, character);
```

**Retro Color Mode**:
Uses 8-color ANSI palette mapped from HSV hue ranges.

**Saturation Boost**:
```c
// Increase saturation by 15% for vibrancy
hsv.saturation = min(hsv.saturation × 1.15, 1.0);
```

---

## GIF Animation System

### Architecture

```
File → Load All Frames → Pre-process → Loop Display
         (stb_image)      (resize +      (frame by
                          sharpen)        frame)
```

### Loading Process

**API**: `stbi_load_gif_from_memory()`

**Implementation**:
```c
gif_animation_t load_gif_animation(const char* file_path) {
    // Read file into memory
    FILE* f = fopen(file_path, "rb");
    unsigned char* file_data = malloc(file_size);
    fread(file_data, 1, file_size, f);
    
    // Load all frames
    int* delays_centiseconds;
    stbi_uc* raw_frames = stbi_load_gif_from_memory(
        file_data, file_size,
        &delays_centiseconds,
        &width, &height, &frame_count, &channels, 0
    );
    
    // Convert to internal format
    for (int i = 0; i < frame_count; i++) {
        normalize_frame(&frames[i], raw_frames, i);
        frames[i].delay = delays_centiseconds[i];
    }
    
    return animation;
}
```

### Pre-Processing Strategy

**Why Pre-process**: Process once, display multiple times for consistent quality and performance.

**Process**:
```c
void play_gif_animation(gif_animation_t* anim, args_t* args) {
    // Pre-process all frames once
    image_t* processed = calloc(frame_count, sizeof(image_t));
    
    for (int i = 0; i < frame_count; i++) {
        // Resize to terminal dimensions
        processed[i] = make_resized(&anim->frames[i], max_width, max_height, char_ratio);
        
        // Apply sharpening (on resized frame)
        if (args->sharpen_strength > 0.0) {
            unsharp_mask(&processed[i], args->sharpen_strength, 1.0);
        }
        
        // Apply grayscale if enabled
        // (handled in print_image function)
    }
    
    // Display loop
    for (int loop = 0; loop < 3; loop++) {
        for (int i = 0; i < frame_count; i++) {
            // Clear and render
            printf("\x1b[H");  // Move cursor to home
            print_image(&processed[i], args);
            fflush(stdout);
            
            // Frame delay (centiseconds to microseconds)
            int delay_ms = max(anim->delays[i] * 10, 20);  // Min 20ms
            usleep(delay_ms * 1000);
        }
    }
    
    // Cleanup
    for (int i = 0; i < frame_count; i++) {
        free_image(&processed[i]);
    }
    free(processed);
}
```

### Timing System

**GIF Delay Format**: Centiseconds (1/100 second)

**Conversion**:
```
Centiseconds → Milliseconds → Microseconds
     10cs    →     100ms     →   100,000μs
```

**Minimum Delay**: 20ms (50 FPS maximum) for smooth playback

---

## Performance Optimizations

### Memory Layout

**Structure Alignment**:
```c
typedef struct {
    size_t width;     // 8 bytes
    size_t height;    // 8 bytes
    int channels;     // 4 bytes
    double* data;     // 8 bytes (pointer)
} image_t;  // Total: 32 bytes (with padding)
```

### Cache Efficiency

**Row-Major Access**:
```c
// Optimal: sequential memory access
for (y = 0; y < height; y++) {
    for (x = 0; x < width; x++) {
        pixel = image[y * width + x];
    }
}
```

### Compiler Optimizations

**Release Build Flags**:
- `-O3`: Maximum optimization
- `-flto`: Link-time optimization
- `-march=native`: CPU-specific optimizations

**Performance Impact**:
- Typical image (100×75): <200ms processing time
- Sharpening overhead: +100-150ms
- GIF animation: 20-50 FPS depending on complexity

---

## Memory Management

### Allocation Strategy

**RAII-style Pattern**:
```c
// Acquire
image_t image = load_image(path);
if (!image.data) return ERROR;

// Use
process(&image);

// Release
free_image(&image);
```

### Memory Safety

**Bounds Checking**:
```c
double* get_pixel(image_t* img, size_t x, size_t y) {
    if (!img || !img->data) return NULL;
    if (x >= img->width || y >= img->height) return NULL;
    
    size_t index = (y * img->width + x) * img->channels;
    return &img->data[index];
}
```

**Zero Initialization**:
```c
image_t img = {0};
double* buffer = calloc(size, sizeof(*buffer));
```

### Leak Prevention

**GIF Animation Cleanup**:
```c
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
```

---

## Build System

### Makefile Targets

```makefile
make              # Development build
make release      # Optimized build (-O3 -flto -march=native)
make clean        # Remove build artifacts
make install      # Install system-wide (requires sudo)
make uninstall    # Remove from system
```

### Compiler Flags

**Development**:
```
-Wall -Wextra -Wpedantic -std=c99 -Iinclude -D_GNU_SOURCE
```

**Release**:
```
-O3 -flto -march=native
```

### CMake Support

```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
sudo cmake --install .
```

### Dependencies

**Build Requirements**:
- GCC or Clang compiler
- Make or CMake
- Standard C library
- Math library (-lm)

**Runtime Requirements**:
- UTF-8 terminal (for braille mode)
- True color support (for 24-bit color)

---

## Technical Specifications

### Processing Pipeline Timing

| Stage | Typical Time | Notes |
|-------|-------------|-------|
| Image Load | 10-30ms | Depends on file size |
| Sharpening | 100-150ms | Optional |
| Resize | 20-50ms | Area averaging |
| Contrast Enhancement | 10-20ms | Histogram equalization |
| Edge Detection | 30-50ms | Sobel operator |
| Character Mapping | 5-10ms | Lookup table |
| Color Output | 10-20ms | ANSI sequences |

### Memory Usage

| Operation | Memory | Notes |
|-----------|--------|-------|
| 1920×1080 image | ~24 MB | RGB double precision |
| 100×75 output | ~180 KB | Processed |
| GIF (30 frames) | ~30 MB | All frames in memory |

### Supported Image Sizes

- **Minimum**: 10×10 pixels
- **Maximum**: Limited by available memory
- **Recommended**: 100×100 to 4000×4000 pixels
- **Output**: 40×30 to 250×187 characters

---

## Version Information

**Current Version**: 2.2.0  
**Release Date**: 2025-10-26  
**License**: MIT  
**Author**: danko12
