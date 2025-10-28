# ASCII-MEDIA v3.0.0

**Terminal ASCII Art Converter - Transform Images & GIFs into Stunning Terminal Art**

[![Platform](https://img.shields.io/badge/Platform-Linux%20%7C%20macOS-blue.svg)]()
[![License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)
[![Build](https://img.shields.io/badge/Build-Passing-brightgreen.svg)]()

---

## 📋 Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Quick Start](#quick-start)
- [Installation](#installation)
- [Usage Guide](#usage-guide)
- [Examples](#examples)
- [Technical Details](#technical-details)
- [Troubleshooting](#troubleshooting)
- [Contributing](#contributing)
- [License](#license)

---

## 🎯 Overview

ASCII-MEDIA adalah aplikasi terminal hybrid C/C++/Rust yang powerful untuk mengkonversi gambar dan GIF menjadi ASCII art berkualitas tinggi. Dibangun dengan fokus pada:

✅ **Perfect Aspect Ratio** - Tidak gepeng! Vertikal tetap vertikal, horizontal tetap horizontal, kotak tetap kotak  
✅ **High Performance** - Optimized C/C++ core dengan Rust orchestration  
✅ **Beautiful Output** - ITU-R BT.601 luminance calculation, gamma compensation  
✅ **Smooth Animation** - 60+ FPS GIF playback dengan double buffering  

### Supported Platforms

- ✅ **Linux** (Ubuntu, Debian, Fedora, Arch, dll)
- ✅ **macOS** (Intel & Apple Silicon)
- ❌ **Windows** (Not supported in v3.0)

---

## ✨ Features

### 🖼️ Image Processing

- **Format Support**: JPEG, PNG, BMP, TIFF, GIF, TGA, PSD, HDR, PIC
- **Color Modes**:
  - 24-bit True Color (16.7M colors)
  - Grayscale mode
  - Retro 8-color palette
- **Enhancement Options**:
  - Unsharp mask sharpening (0.0-2.0)
  - Edge detection dengan Sobel operator
  - Adaptive contrast enhancement
- **Character Palettes**:
  - Simple (12 characters)
  - Standard (40 characters)  
  - Enhanced (70+ characters)
- **Aspect Ratio Correction**: Automatic terminal character ratio compensation (2:1) untuk hasil yang sempurna

### 🎬 GIF Animation

- **Ultra-Smooth Playback**: 60+ FPS dengan double buffering
- **Precise Timing**: Frame delays dari metadata GIF
- **Pre-Processing**: Resize dan sharpen sekali untuk performa optimal
- **No Flicker**: Advanced frame buffering technique

### 🎨 Display Options

- **Braille Mode**: Ultra-high detail rendering (experimental)
- **Custom Dimensions**: Set width/height manual atau auto-detect
- **Terminal Resize**: SIGWINCH handler untuk adaptasi real-time
- **Graceful Shutdown**: SIGINT handler dengan cleanup

---

## 🚀 Quick Start

### Prerequisites

**Linux (Debian/Ubuntu):**
```bash
sudo apt update
sudo apt install build-essential gcc g++ cmake
```

**Linux (Fedora/RHEL):**
```bash
sudo dnf groupinstall "Development Tools"
```

**macOS:**
```bash
xcode-select --install
```

### Build

```bash
# Clone repository (jika dari GitHub)
git clone https://github.com/yourusername/ascii-media.git
cd ascii-media

# Build
make release

# Test
./ascii sample-images/puffin.jpg -D 3
```

**Build output:**
```
╔══════════════════════════════════════════════════════════════╗
║          ASCII-MEDIA v3.0.0 Build Complete                   ║
╚══════════════════════════════════════════════════════════════╝
Binaries ready:
-rwxr-xr-x 1 user user 182K ascii
-rwxr-xr-x 1 user user 884K ascii-media
```

---

## 📦 Installation

### Method 1: Makefile (Recommended)

```bash
# Build and install system-wide
make release
sudo make install

# Uninstall
sudo make uninstall
```

Binaries akan terinstall di `/usr/local/bin/`:
- `ascii` - Image/GIF processor
- `ascii-media` - Main orchestrator

### Method 2: CMake

```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
sudo cmake --install .
```

### Verification

```bash
which ascii ascii-media
ascii --version
```

---

## 📖 Usage Guide

### Basic Command Structure

```bash
# Direct image processor
./ascii <image-file> [OPTIONS]

# Via orchestrator (auto-routing)
./ascii-media <media-file> [OPTIONS]
```

### Command-Line Options

| Option | Short | Description | Default | Example |
|--------|-------|-------------|---------|---------|
| `--help` | `-h` | Show help message | - | `./ascii -h` |
| `--version` | `-v` | Show version | - | `./ascii -v` |
| `-D <preset>` | - | Dimension preset (1-6) | Auto | `-D 3` |
| `-mw <width>` | - | Max width in chars | Auto | `-mw 100` |
| `-mh <height>` | - | Max height in chars | Auto | `-mh 50` |
| `--sharpen <val>` | `-s` | Sharpening (0.0-2.0) | 0.0 | `-s 1.2` |
| `-et <threshold>` | - | Edge detection (0.0-4.0) | 4.0 | `-et 2.0` |
| `-cr <ratio>` | - | Character aspect ratio | 2.0 | `-cr 2.0` |
| `--braille` | - | Use braille characters | Off | `--braille` |
| `--retro-colors` | - | 8-color palette | Off | `--retro-colors` |
| `--grayscale` | - | Black & white mode | Off | `--grayscale` |
| `--animate` | - | Animate GIF files | Off | `--animate` |
| `--debug` | - | Show debug info | Off | `--debug` |

### Dimension Presets

Preset `-D` memberikan quick access ke ukuran pre-defined dengan aspect ratio 4:1 yang sudah dikoreksi:

| Preset | Width | Height | Characters | Use Case |
|--------|-------|--------|------------|----------|
| `-D 1` | 40 | 15 | 600 | Quick preview, small terminal |
| `-D 2` | 64 | 24 | 1,536 | Default, compact view |
| `-D 3` | 100 | 37 | 3,700 | **Recommended**, standard detail |
| `-D 4` | 150 | 56 | 8,400 | High detail, large terminal |
| `-D 5` | 200 | 75 | 15,000 | Very high detail, optimal for braille |
| `-D 6` | 250 | 93 | 23,250 | Maximum detail, ultra-wide screen |

**Note**: Height otomatis dikoreksi dengan formula `height = width / 4` untuk mencegah vertical stretching.

---

## 💡 Examples

### 1. Basic Image Display

```bash
# Standard rendering dengan preset 3
./ascii sample-images/puffin.jpg -D 3

# Full auto-detect terminal size
./ascii sample-images/mountain.jpg

# Custom dimensions
./ascii sample-images/lion.jpg -mw 120 -mh 45
```

**Output:**
```
=++=++++***+
+===+***+***
===++***++++
==+****+++++
=*****++++=+
...
```

### 2. Color Modes

```bash
# Full 24-bit truecolor (default)
./ascii sample-images/puffin.jpg -D 3

# Grayscale (black & white)
./ascii sample-images/puffin.jpg -D 3 --grayscale

# Retro 8-color palette
./ascii sample-images/puffin.jpg -D 3 --retro-colors
```

### 3. Image Enhancement

```bash
# Sharpening (subtle)
./ascii sample-images/penguin.jpg -D 3 --sharpen 0.8

# Sharpening (strong)
./ascii sample-images/lion.jpg -D 4 --sharpen 1.5

# Edge detection
./ascii sample-images/kontrast.jpg -D 4 -et 2.0

# Combined: sharpen + edge detection
./ascii sample-images/mountain.jpg -D 4 --sharpen 1.0 -et 2.5
```

**Sharpening Guidelines:**
- `0.5-1.0`: Subtle enhancement, portraits
- `1.0-1.5`: Standard sharpening, most images
- `1.5-2.0`: Aggressive, may introduce artifacts

**Edge Threshold Guidelines:**
- `3.0-4.0`: Minimal effect, subtle edges
- `2.0-2.5`: Moderate edge emphasis
- `1.0-1.5`: Strong edges, architecture
- `0.5-1.0`: Very aggressive, line art effect

### 4. Braille Mode (High Detail)

```bash
# Standard braille
./ascii sample-images/penguin.jpg -D 5 --braille

# Braille dengan sharpening
./ascii sample-images/lion.jpg -D 5 --braille --sharpen 0.8

# Grayscale braille
./ascii sample-images/puffin.jpg -D 5 --braille --grayscale
```

**Braille Requirements:**
- UTF-8 terminal
- Font dengan braille support (DejaVu Sans Mono, Fira Code)
- Recommended preset: `-D 5` atau `-D 6`

### 5. GIF Animation

```bash
# Static GIF (first frame only)
./ascii sample-images/nyan-cat.gif -D 3

# Animated playback
./ascii sample-images/nyan-cat.gif -D 3 --animate

# Animated dengan effects
./ascii sample-images/nyan-cat.gif -D 2 --animate --sharpen 0.5

# Animated grayscale
./ascii sample-images/nyan-cat.gif -D 3 --animate --grayscale
```

**GIF Performance Tips:**
- Gunakan preset lebih kecil (`-D 2` atau `-D 3`) untuk playback lebih smooth
- Preset besar (`-D 5`, `-D 6`) dapat menyebabkan lag pada sistem lambat
- Sharpening menambah processing time per frame

### 6. Aspect Ratio Scenarios

```bash
# Portrait image (vertikal)
./ascii portrait.jpg -D 3
# Output: Tetap vertikal, tidak gepeng horizontal

# Landscape image (horizontal)
./ascii landscape.jpg -D 3
# Output: Tetap horizontal, tidak gepeng vertikal

# Square image
./ascii square.jpg -D 3
# Output: Tetap kotak, tidak distorted
```

**Aspect Ratio Verification:**
- Program akan warning jika deviation > 3%
- Formula: `aspect_corrected = (width / height) / character_ratio`
- Character ratio = 2.0 (default, terminal chars adalah 2x tinggi vs lebar)

---

## 🔧 Technical Details

### Architecture

```
ASCII-MEDIA v3.0.0
├── C Core (src/*.c)
│   ├── image.c           - STB image loading, resize, GIF
│   ├── argparse.c        - CLI argument parsing
│   └── print_image.c     - Terminal rendering, animation
├── C++ Optimization (src/*.cpp)
│   └── ascii_processor.cpp - Bilinear interpolation, gamma
├── Rust Orchestration (rust/src/)
│   └── main.rs           - Media routing, CLI interface
└── Build System
    ├── Makefile          - Main build orchestration
    ├── CMakeLists.txt    - C/C++ compilation
    └── Cargo.toml        - Rust compilation
```

### Image Processing Pipeline

```
┌──────────────┐
│ Input Image  │ (JPEG/PNG/GIF/etc)
└──────┬───────┘
       │
       ↓
┌──────────────┐
│ STB Decode   │ RGB(A) data, all channels
└──────┬───────┘
       │
       ↓
┌──────────────┐
│ Optional:    │ Unsharp mask (amount, radius)
│ Sharpen      │
└──────┬───────┘
       │
       ↓
┌──────────────┐
│ Aspect Ratio │ img_aspect = W / (H * char_ratio)
│ Calculation  │ char_ratio = 2.0 default
└──────┬───────┘
       │
       ↓
┌──────────────┐
│ Fit to Term  │ Maintain aspect ratio
│ Resize       │ Area averaging / bilinear
└──────┬───────┘
       │
       ↓
┌──────────────┐
│ Luminance    │ ITU-R BT.601: 0.299R + 0.587G + 0.114B
│ Calculation  │ Gamma correction: L^(1/2.2)
└──────┬───────┘
       │
       ↓
┌──────────────┐
│ Optional:    │ Sobel operator (Gx, Gy)
│ Edge Detect  │
└──────┬───────┘
       │
       ↓
┌──────────────┐
│ Character    │ Luminance → Palette index
│ Mapping      │ 70+ character gradient
└──────┬───────┘
       │
       ↓
┌──────────────┐
│ Color        │ True color / Retro / Grayscale
│ Application  │ ANSI escape codes
└──────┬───────┘
       │
       ↓
┌──────────────┐
│ Terminal     │ Stdout with flush
│ Render       │
└──────────────┘
```

### GIF Animation Processing

```
┌──────────────┐
│ GIF File     │
└──────┬───────┘
       │
       ↓
┌──────────────┐
│ STB GIF Load │ Extract all frames + delays
└──────┬───────┘
       │
       ↓
┌──────────────┐
│ Pre-Process  │ Resize & sharpen ALL frames once
│ All Frames   │ (Not per-display for performance)
└──────┬───────┘
       │
       ↓
┌──────────────┐
│ Loop Playback│ 3x default
│              │
│  ┌────────────────────┐
│  │ For each frame:    │
│  │ 1. Move cursor home│
│  │ 2. Render frame    │
│  │ 3. Flush stdout    │
│  │ 4. nanosleep(delay)│
│  └────────────────────┘
└──────┬───────┘
       │
       ↓
┌──────────────┐
│ Cleanup      │ Free all frames
└──────────────┘
```

### Aspect Ratio Correction Formula

```c
// Input
double img_width = original->width;
double img_height = original->height;
double character_ratio = 2.0;  // Terminal chars are 2x taller

// Step 1: Koreksi aspect ratio untuk terminal
double img_aspect_corrected = img_width / (img_height * character_ratio);

// Step 2: Fit to available space
double terminal_aspect = max_width / max_height;

if (img_aspect_corrected > terminal_aspect) {
    // Width-limited
    width = max_width;
    height = (int)(max_width / img_aspect_corrected + 0.5);
} else {
    // Height-limited
    height = max_height;
    width = (int)(max_height * img_aspect_corrected + 0.5);
}

// Step 3: Verify (optional)
double final_aspect = (width / height) * character_ratio;
double deviation = abs((final_aspect - original_aspect) / original_aspect);
// Warning if deviation > 3%
```

### Performance Characteristics

| Preset | Image (1920x1080) | GIF (24 frames) | Memory | Notes |
|--------|-------------------|-----------------|--------|-------|
| `-D 1` | <0.1s | 0.5s | 1 MB | Fastest, lowest quality |
| `-D 2` | <0.2s | 1.0s | 2 MB | Default, balanced |
| `-D 3` | 0.3s | 2.0s | 5 MB | Recommended |
| `-D 4` | 0.8s | 5.0s | 12 MB | High detail |
| `-D 5` | 1.5s | 10.0s | 22 MB | Very high, good for braille |
| `-D 6` | 2.5s | 18.0s | 35 MB | Maximum, slow systems may lag |

*Tested on: Intel i5-8250U, 8GB RAM, SSD*

---

## 🛠️ Troubleshooting

### Build Issues

**Problem**: `cmake: command not found`  
**Solution**:
```bash
# Linux
sudo apt install cmake

# macOS
brew install cmake
```

**Problem**: `gcc: command not found`  
**Solution**: Install build tools (see [Prerequisites](#prerequisites))

**Problem**: Undefined reference to `sqrt`  
**Solution**: Make sure to use Makefile (includes `-lm` flag automatically)

### Runtime Issues

**Problem**: Colors tidak muncul / garbled  
**Solution**:
```bash
# Test true color support
echo -e "\x1b[38;2;255;100;0mTRUECOLOR\x1b[0m"

# If not orange, use:
./ascii image.jpg --retro-colors
```

**Problem**: Braille characters tampil sebagai boxes  
**Solution**:
```bash
# Set UTF-8 locale
export LANG=en_US.UTF-8
export LC_ALL=en_US.UTF-8

# Install font dengan braille support
# Rekomendasi: DejaVu Sans Mono, Fira Code
```

**Problem**: Output terlalu besar / kecil  
**Solution**:
```bash
# Auto-detect
./ascii image.jpg

# Manual adjustment
./ascii image.jpg -mw 100 -mh 40
```

**Problem**: GIF animation stuttering / lag  
**Solution**:
```bash
# Use smaller preset
./ascii animation.gif --animate -D 2

# Disable sharpening
./ascii animation.gif --animate -D 3
```

**Problem**: Aspect ratio warning `⚠️ Aspect ratio deviation: X%`  
**Solution**:
- Jika X < 5%: Acceptable, slight distortion
- Jika X > 5%: Check terminal size, try different preset
- Adjust character ratio: `-cr 1.8` or `-cr 2.2`

### Platform-Specific

**macOS: "xcode-select: error"**
```bash
xcode-select --install
# Follow on-screen instructions
```

**Linux: Permission denied on install**
```bash
sudo make install
# Or install locally
mkdir -p ~/.local/bin
cp ascii ascii-media ~/.local/bin/
export PATH="$HOME/.local/bin:$PATH"
```

---

## 🤝 Contributing

Contributions welcome! Project ini menggabungkan:
- ASCII image converter (C/C++ core)
- Rust orchestration untuk future expansion

### Development Setup

```bash
# Clone & build
git clone <repo>
cd ascii-media
make release

# Run tests
make test

# Clean build
make clean
```

### Code Style

- **C code**: Follow existing style (K&R with 4-space indent)
- **C++ code**: std::c++17, RAII principles
- **Rust code**: `cargo fmt`, follow Rust conventions

### Testing

```bash
# Test all presets
for preset in {1..6}; do
    ./ascii sample-images/puffin.jpg -D $preset
done

# Test aspect ratios
./ascii sample-images/portrait.jpg -D 3  # Vertical
./ascii sample-images/landscape.jpg -D 3  # Horizontal
./ascii sample-images/square.jpg -D 3     # Square

# Test GIF
./ascii sample-images/nyan-cat.gif --animate -D 3
```

---

## 📄 License

MIT License - see [LICENSE](LICENSE) file for details.

Copyright (c) 2025 danko12

---

## 🙏 Acknowledgments

- **STB Libraries** - Sean Barrett's excellent single-header libraries
- **ascii-video-player** - Inspiration for GIF handling
- **Community** - All contributors and users

---

## 📞 Support

- **Issues**: [GitHub Issues](https://github.com/yourusername/ascii-media/issues)
- **Documentation**: See `TECHNICAL.md` for detailed technical docs
- **Examples**: Check `sample-images/` directory

---

## 🗺️ Roadmap

### v3.1 (Planned)
- [ ] Video file support (MP4, AVI, MKV)
- [ ] Audio extraction and playback sync
- [ ] Custom color palette import


---

**Made By danko12**

