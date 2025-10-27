# ASCII-MEDIA v3.0.0

**Advanced multimedia ASCII converter - Transform images, GIFs, videos, and webcam into stunning terminal art**

**A hybrid C/C++/Rust terminal multimedia player - Linux & macOS only**

[![Platform](https://img.shields.io/badge/Platform-Linux%20%7C%20macOS-blue.svg)]()
[![License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)

---

## Overview

ASCII-MEDIA is a powerful hybrid C/C++/Rust application that transforms images, GIFs, videos, and live webcam streams into high-quality ASCII art. Built exclusively for Linux and macOS with performance and accuracy in mind, it features ITU-R BT.601 luminance calculations with gamma compensation, smart aspect ratio lock (±3% tolerance), dynamic terminal resize detection (SIGWINCH), graceful shutdown (SIGINT), and full tplay integration for video/webcam/streaming support.

The project combines:
- **C Core** - Efficient image loading and processing using STB libraries
- **C++ Optimization** - Advanced algorithms for superior image quality
- **Rust Integration** - Full video, webcam, and YouTube streaming support via tplay

### Core Features

**Multimedia Support:**
- **Images** - ✅ JPEG, PNG, BMP, TIFF, WEBP (24-bit True Color) - **FULLY WORKING**
- **GIF Animation** - ✅ Ultra-smooth 60+ FPS with double buffering - **FULLY WORKING**
- **Video Playback** - ⏳ MP4, AVI, MKV, WEBM via OpenCV + FFmpeg - **Requires build with --features video**
- **Webcam Streaming** - ⏳ Live /dev/video* device support - **Requires build with --features webcam**
- **YouTube Integration** - ⏳ Direct URL playback with yt-dlp - **Requires build with --features youtube**

**Advanced Rendering:**
- **BT.601 Luminance** - ITU-R BT.601 standard (0.299R + 0.587G + 0.114B) with gamma compensation (L_gamma = L^(1/2.2))
- **Enhanced Character Palettes** - Simple (12 chars), Standard (40 chars), Enhanced (70+ chars)
- **Smart Aspect Ratio Lock** - Progressive correction with ±3% tolerance prevents distortion
- **Dynamic Terminal Resize** - SIGWINCH handler auto-adjusts to terminal size changes
- **Graceful Shutdown** - SIGINT handler with proper cleanup message
- **Double Buffering for GIF** - Full frame construction before display eliminates flicker
- **Debug Mode** - Real-time stats: FPS, terminal size, frame dimensions

**Display Options:**
- **24-bit True Color** - 16.7 million colors for photorealistic ASCII
- **Retro Color Mode** - 3-bit palette (8 colors) for vintage terminal look
- **Grayscale Mode** - Classic monochrome ASCII art
- **Braille Characters** - Ultra-high detail rendering (experimental)

**Platform:**
- **Linux & macOS Only** - Windows support removed in v3.0.0

---

## Quick Start

### Prerequisites

**Linux (Debian/Ubuntu):**
```bash
sudo apt update
sudo apt install build-essential gcc g++ libopencv-dev ffmpeg libavcodec-dev libavformat-dev libswscale-dev
```

**macOS:**
```bash
brew install gcc opencv ffmpeg
```

### Build and Install

```bash
# Build C/C++ components (images & GIFs)
make release

# Build Rust components (video & webcam) - optional
cargo build --release --features full

# Install system-wide (Linux/macOS)
sudo make install

# Run
./ascii sample-images/puffin.jpg -D 5 --enhanced-palette
```

**Note**: BT.601 luminance with smart aspect ratio lock (±3% tolerance) for perfect proportions.

---

## Usage

### Basic Command

```bash
./ascii <image-file> [OPTIONS]
```

### Options

| Option | Description | Default |
|--------|-------------|---------|
| `-D <preset>` | Dimension preset (1-6) | Auto or 2 |
| `-mw <width>` | Maximum width in characters | Auto-detect or 64 |
| `-mh <height>` | Maximum height in characters | Auto-detect or 48 |
| `-s, --sharpen <strength>` | Sharpening strength (0.0-2.0) | 0.0 (disabled) |
| `-et <threshold>` | Edge detection threshold (0.0-4.0) | 4.0 (disabled) |
| `-cr <ratio>` | Character aspect ratio | 2.0 |
| `--braille` | Use braille characters | Off |
| `--retro-colors` | Use 8-color palette | Off |
| `--grayscale` | Black and white mode | Off |
| `--animate` | Animate GIF files | Off |
| `-h, --help` | Show help message | - |
| `-v, --version` | Show version | - |

### Dimension Presets

The `-D` flag provides quick access to predefined output sizes:

| Preset | Size (4:1 ratio) | Characters | Use Case |
|--------|------------------|------------|----------|
| **1** | 40×15 | 600 | Quick preview |
| **2** | 64×24 | 1,536 | Default, compact view |
| **3** | 100×37 | 3,700 | Standard detail (recommended) |
| **4** | 150×56 | 8,400 | High detail |
| **5** | 200×75 | 15,000 | Very high detail, optimal for braille |
| **6** | 250×93 | 23,250 | Maximum detail |

**Note**: Heights are optimized for 4:1 aspect ratio to prevent vertical stretching in terminal output.

**Custom Dimensions**: Use `-mw` and `-mh` for precise control over output size. These flags override preset values when specified after `-D`.

---

## Examples

### Basic Usage

```bash
# Standard rendering
./ascii sample-images/puffin.jpg -D 3

# High detail
./ascii sample-images/lion.jpg -D 4

# Custom dimensions
./ascii sample-images/kontrast.jpg -mw 120 -mh 90
```

### Color Modes

```bash
# Full 24-bit color (default)
./ascii sample-images/puffin.jpg -D 3

# Grayscale (black and white)
./ascii sample-images/puffin.jpg -D 3 --grayscale

# Retro 8-color palette
./ascii sample-images/puffin.jpg -D 3 --retro-colors
```

### Enhancement

```bash
# Apply sharpening
./ascii sample-images/penguin.jpg -D 3 --sharpen 1.2

# Edge detection
./ascii sample-images/kontrast.jpg -D 4 -et 2.0

# Combined sharpening and edge detection
./ascii sample-images/lion.jpg -D 4 --sharpen 1.0 -et 2.5
```

### Braille Mode

```bash
# Standard braille rendering
./ascii sample-images/penguin.jpg -D 5 --braille

# Braille with sharpening
./ascii sample-images/lion.jpg -D 5 --braille --sharpen 0.8

# Grayscale braille
./ascii sample-images/puffin.jpg -D 5 --braille --grayscale
```

### GIF Animation

```bash
# Static GIF (first frame)
./ascii sample-images/nyan-cat.gif -D 3

# Animated playback
./ascii sample-images/nyan-cat.gif -D 3 --animate

# Animated with effects
./ascii sample-images/nyan-cat.gif -D 2 --animate --sharpen 0.5
```

---

## Installation

### Prerequisites

#### Linux - Debian/Ubuntu

```bash
sudo apt update
sudo apt install build-essential gcc make
```

#### Linux - Fedora/RHEL/CentOS

```bash
sudo dnf groupinstall "Development Tools"
```

#### Linux - Arch

```bash
sudo pacman -S base-devel
```

#### macOS

```bash
xcode-select --install
```

#### Windows

Install MinGW-w64:
1. Download from https://www.mingw-w64.org/downloads/
2. Install to `C:\mingw64`
3. Add `C:\mingw64\bin` to system PATH

### Build Instructions

#### Linux/macOS

```bash
# Development build
make

# Optimized release build
make release

# Install system-wide
sudo make install

# Uninstall
sudo make uninstall
```

#### Windows (MinGW)

```cmd
REM Build
build.bat

REM Install (run as Administrator)
install.bat
```

#### Using CMake

```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
sudo cmake --install .
```

---

## Advanced Usage

### Color Mode Selection

The tool provides three color rendering modes:

**Full Color (Default)**: 24-bit RGB color using ANSI escape sequences. Requires terminal with true color support.

```bash
ascii image.jpg -D 3
```

**Grayscale Mode**: Converts image to black and white, preserving luminance information.

```bash
ascii image.jpg -D 3 --grayscale
```

**Retro Color Mode**: Uses 3-bit color palette (8 colors) for compatibility with older terminals.

```bash
ascii image.jpg -D 3 --retro-colors
```

### Sharpening

Sharpening uses an unsharp mask algorithm to enhance edge definition:

- **0.5-1.0**: Subtle enhancement, good for portraits
- **1.0-1.5**: Standard sharpening, works for most images
- **1.5-2.0**: Aggressive sharpening, may introduce artifacts

```bash
ascii photo.jpg -D 3 --sharpen 1.2
```

### Edge Detection

Edge detection applies Sobel operator to emphasize boundaries:

- **3.0-4.0**: Subtle edges, minimal effect
- **2.0-2.5**: Moderate edge emphasis
- **1.0-1.5**: Strong edges, architectural photos
- **0.5-1.0**: Very aggressive, line art effect

```bash
ascii architecture.jpg -D 4 -et 1.8
```

### Braille Rendering

Braille mode uses Unicode braille patterns (U+2800 to U+28FF) for 2x effective resolution:

- Recommended with preset 5 or 6 for optimal results
- Requires UTF-8 terminal and font with braille support
- Lower sharpening values (0.5-1.0) work best with braille

```bash
ascii detailed-image.jpg -D 5 --braille --sharpen 0.8
```

### Character Aspect Ratio

The `-cr` flag adjusts for terminal font aspect ratio. Default is 2.0 (character height is 2x width). Adjust if output appears stretched:

```bash
# If output looks too tall
ascii image.jpg -D 3 -cr 1.8

# If output looks too wide
ascii image.jpg -D 3 -cr 2.2
```

### GIF Animation

Animated GIF playback preserves frame timing from source file:

- Use smaller presets (2-3) for smoother playback
- Large presets (5-6) may cause frame lag on slower systems
- Sharpening adds processing time per frame

```bash
ascii animation.gif -D 2 --animate
```

---

## Terminal Configuration

### True Color Support

Test terminal true color capability:

```bash
echo -e "\x1b[38;2;255;100;0mTRUECOLOR\x1b[0m"
```

If text displays as orange, terminal supports 24-bit color. If not, use `--retro-colors` flag.

### UTF-8 Support

For braille mode, ensure UTF-8 locale:

```bash
locale | grep UTF-8
export LANG=en_US.UTF-8
export LC_ALL=en_US.UTF-8
```

### Font Recommendations

Monospace fonts with good Unicode coverage:
- DejaVu Sans Mono
- Consolas (Windows)
- Monaco (macOS)
- Fira Code
- JetBrains Mono

### Optimal Display Settings

- **Font size**: 8-12pt for standard viewing, 6-8pt for maximum detail
- **Background**: Dark backgrounds generally produce better contrast
- **Terminal size**: Ensure terminal window can accommodate output dimensions

---

## Technical Details

### Image Processing Pipeline

1. **Load**: Image decoded using stb_image library
2. **Sharpen**: Optional unsharp mask applied to source image
3. **Resize**: Aspect-preserving downsampling with bilinear interpolation
4. **Analyze**: Gamma-corrected luminance calculation (ITU-R BT.709)
5. **Contrast**: Optional histogram equalization
6. **Edge Detect**: Optional Sobel operator with directional mapping
7. **Character Map**: Luminance-to-character mapping with perceptual optimization
8. **Color**: RGB color preservation through HSV color space
9. **Render**: ANSI escape sequence generation

### Supported Image Formats

- JPEG (.jpg, .jpeg)
- PNG (.png)
- BMP (.bmp)
- GIF (.gif) - static and animated
- TGA (.tga)
- PSD (.psd) - rasterized layers
- HDR (.hdr)
- PIC (.pic)

### Performance Characteristics

Processing time varies by preset size and options:

| Preset | Typical Time | With Sharpening | With Animation |
|--------|-------------|-----------------|----------------|
| 1-2 | <1s | 1-2s | 2-5s |
| 3 | 1-2s | 2-3s | 5-10s |
| 4 | 2-4s | 4-6s | 10-20s |
| 5-6 | 5-10s | 10-15s | 20-40s |

Times are approximate for typical images on modern hardware.

---

## Troubleshooting

### Build Issues

**"gcc: command not found"**

Install compiler toolchain for your platform (see Prerequisites section).

**"undefined reference to 'sqrt'"**

Use provided makefile which includes `-lm` linker flag.

### Runtime Issues

**No colors displayed**

Terminal lacks true color support. Use `--retro-colors` flag.

**Braille characters show as boxes**

Terminal lacks UTF-8 or braille font support. Set UTF-8 locale and install compatible font.

**GIF animation stutters**

Use smaller preset (2 or 3) or disable sharpening to reduce per-frame processing time.

**Output too large/small**

Adjust with custom dimensions: `-mw <width> -mh <height>`

### Platform-Specific

**Windows: Missing DLL errors**

Add MinGW bin directory to PATH or copy required DLLs to executable directory:
```cmd
copy C:\mingw64\bin\libgcc_s_seh-1.dll .
copy C:\mingw64\bin\libstdc++-6.dll .
copy C:\mingw64\bin\libwinpthread-1.dll .
```

**Linux/macOS: Permission denied on install**

Use `sudo` for system-wide installation:
```bash
sudo make install
```

---

## Technical Architecture

This project uses a hybrid C/C++/Rust architecture:

- **C Core (`src/*.c`)** - Fast image loading, resizing, and transformations
- **C++ Module (`src/ascii_processor.cpp`)** - Advanced image processing with bilinear interpolation and gamma-correct luminance
- **Rust Layer (`rust/`)** - Foundation for future video/webcam/streaming features
- **Build System** - Supports both Make and CMake for maximum compatibility

## Future Features

- 🎥 Video file support (MP4, AVI, etc.)
- 📹 Webcam streaming to ASCII
- 🔗 YouTube video playback
- 🎵 Audio synchronization
- 🌐 Network streaming

## Contributing

Contributions welcome! This project combines:
- The ASCII image converter (originally by danko12)
- Video/webcam player architecture (inspired by tplay)

## License

MIT License - see LICENSE file for details.

Copyright (c) 2025 danko12

---

## Version

Current version: **2.3.0**

See CHANGELOG.md for version history and release notes.
