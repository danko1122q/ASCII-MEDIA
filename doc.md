# ASCII Image Converter - Project Documentation

## Overview
ASCII Image Converter is a professional command-line tool that converts images into high-quality colorized ASCII art. It supports various image formats and provides advanced features including sharpening, braille rendering, edge detection, and grayscale conversion.

## Recent Changes

### Version 2.2.0 (2025-10-26)
- ✅ **NEW**: Grayscale mode with `--grayscale` flag
- ✅ **IMPROVED**: Smoother GIF animation playback (up to 50 FPS)
- ✅ **IMPROVED**: Enhanced frame timing and reduced flicker
- ✅ Complete documentation rewrite in professional English
- ✅ Removed platform-specific references for clean GitHub distribution
- ✅ Updated all examples and usage guides

### Version 2.1.0 (2025-10-26)
- ✅ **NEW**: Dimension preset system with `-D` flag (6 presets: 1-6)
- ✅ Predefined sizes from Tiny (40×30) to XXLarge (250×187)
- ✅ Extensive documentation updates with detailed guides
- ✅ Fixed compiler warnings (unused parameter in unsharp_mask)
- ✅ Updated examples with dimension preset usage
- ✅ Enhanced help message with preset table
- ✅ Multi-platform installation support (Windows, Linux, macOS)
- ✅ Comprehensive testing: All features validated

### Version 2.0.0 (2025-10-26)
- ✅ **FIXED**: GIF animation frame processing bug
- ✅ Unsharp mask sharpening for maximum clarity
- ✅ Braille character mode for ultra-high detail
- ✅ Full animated GIF support with stb_image GIF API
- ✅ Renamed command from `ascii-view` to `ascii`
- ✅ Added flags: `--sharpen`, `--braille`, `--animate`, `--grayscale`

## Project Architecture
- **Language**: C99
- **Build System**: Make (CMake supported)
- **Dependencies**: 
  - GCC/Clang compiler
  - stb_image library (header-only, included)
  - Math library (-lm)
  - UTF-8 terminal (for braille mode)

## Key Features

### Image Processing
- **Gamma-aware luminance**: ITU-R BT.709 standard
- **Adaptive contrast**: Histogram equalization (CLAHE)
- **Unsharp mask sharpening**: Professional-grade enhancement
- **Sobel edge detection**: Directional character mapping
- **HSV color space**: Accurate color preservation
- **Grayscale conversion**: Black-and-white ASCII art

### Rendering Modes
- **ASCII Mode**: 11-level character ramp (` .:-=+*#%@`)
- **Braille Mode**: 8-level Unicode braille patterns (⠀⠁⠃⠇⠏⠟⠿⣿)
- **Edge Mode**: Directional characters (|/-\) for edges
- **Retro Mode**: 8-color vintage palette
- **Grayscale Mode**: Monochrome black-and-white output

### Supported Formats
- JPEG/JPG, PNG, BMP, GIF (static and animated)
- TGA, PSD, HDR, PIC, PNM

## Project Structure
```
ascii-image-converter/
├── src/               # Source code
│   ├── main.c        # Entry point
│   ├── argparse.c    # CLI argument parsing
│   ├── image.c       # Image processing & sharpening
│   └── print_image.c # ASCII/Braille rendering & GIF animation
├── include/          # Header files
│   ├── argparse.h
│   ├── image.h
│   ├── print_image.h
│   └── stb_image.h   # Image loading library
├── sample-images/    # Test images
├── showcase-images/  # Documentation images
├── makefile          # Build configuration
├── CMakeLists.txt    # CMake build file
├── CHANGELOG.md      # Version history
├── README.md         # Full documentation
├── QUICK_START.md    # Quick start guide
├── TECHNICAL.md      # Technical documentation
└── TESTING_REPORT.md # Test results
```

## Usage Examples

### Basic Usage
```bash
./ascii sample-images/puffin.jpg -D 3
```

### With Sharpening
```bash
./ascii sample-images/penguin.jpg --sharpen 1.2 -D 3
```

### Grayscale Mode
```bash
./ascii sample-images/lion.jpg --grayscale -D 3
```

### Braille Mode (Ultra Detail)
```bash
./ascii sample-images/kontrast.jpg --braille -D 5
```

### Edge Detection + Sharpening
```bash
./ascii sample-images/lion.jpg -et 2.0 --sharpen 1.0 -D 4
```

### Animated GIF
```bash
./ascii sample-images/nyan-cat.gif --animate -D 2
```

### Retro Mode
```bash
./ascii sample-images/kontrast.jpg --retro-colors -D 3
```

## Build Commands
```bash
make              # Development build
make release      # Optimized build (-O3 -flto -march=native)
make clean        # Clean build artifacts
make install      # Install system-wide (requires sudo)
make help         # Show all make targets
```

## Command-Line Options

| Flag | Description | Default |
|------|-------------|---------|
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
| `-h, --help` | Show help | - |
| `-v, --version` | Show version | - |

### Dimension Presets

| Preset | Command | Dimensions | Characters | Use Case |
|--------|---------|------------|------------|----------|
| 1 | `-D 1` | 40×30 | 1,200 | Quick preview |
| 2 | `-D 2` | 64×48 | 3,072 | Compact view |
| 3 | `-D 3` | 100×75 | 7,500 | **Standard detail (recommended)** |
| 4 | `-D 4` | 150×112 | 16,800 | High detail |
| 5 | `-D 5` | 200×150 | 30,000 | **Optimal for braille** |
| 6 | `-D 6` | 250×187 | 46,750 | Maximum detail |

## Performance Tips

### Sharpening Guide
- **0.5-0.8**: Light (portraits, soft images)
- **1.0-1.5**: Medium (general photography)
- **1.5-2.0**: Heavy (architecture, text)

### Braille Mode
- Best for detailed images with preset 5 or 6
- Requires UTF-8 terminal
- Disable edge detection for cleaner output
- Lower sharpening values (0.5-1.0) work best

### Edge Detection
- **1.0-1.5**: Strong edges (architecture)
- **2.0-2.5**: Moderate edges (general)
- **3.0-4.0**: Subtle edges (portraits)

### Grayscale Mode
- Works with all other modes (braille, edge detection, retro)
- Preserves luminance and contrast
- Excellent for artistic black-and-white effects
- Can be combined with sharpening for enhanced detail

## Terminal Requirements

### Recommended
- True color support (24-bit)
- UTF-8 encoding (for braille)
- Monospace font with braille characters
- Dark background for better contrast

### Tested Terminals
- ✅ Linux: gnome-terminal, konsole, xterm, tilix
- ✅ macOS: Terminal.app, iTerm2
- ✅ Windows: Windows Terminal, WSL

## Technical Implementation

### Rendering Pipeline
1. Load image using stb_image
2. Apply unsharp mask sharpening (optional)
3. Resize with area averaging
4. Calculate gamma-aware luminance
5. Enhance contrast (CLAHE)
6. Detect edges (Sobel operator)
7. Apply grayscale conversion (if enabled)
8. Map to characters (ASCII/Braille)
9. Output with ANSI 24-bit colors

### GIF Animation Pipeline
1. Load all frames using `stbi_load_gif_from_memory()`
2. Pre-process frames once:
   - Resize to terminal size
   - Apply sharpening (on resized frame)
   - Apply grayscale conversion (if enabled)
3. Display loop with accurate frame timing
4. Cleanup all frames

### Algorithms
- **Unsharp Mask**: Gaussian blur + weighted difference
- **Luminance**: ITU-R BT.709 (0.2126R + 0.7152G + 0.0722B)
- **Contrast**: Histogram equalization (60% enhanced + 40% original)
- **Edge**: Sobel operator with magnitude threshold
- **Color**: RGB → HSV → RGB with saturation boost
- **Grayscale**: Luminance-based conversion with gamma correction

## Development Notes
- C99 standard for maximum compatibility
- Zero runtime dependencies (stb_image is header-only)
- Optimized with -O3 -flto -march=native
- Memory-safe with proper error handling
- Cross-platform compatible (Linux, macOS, Windows)

## License
MIT License - Copyright (c) 2025 danko12

## Author
**danko12** - Creator and maintainer

---

**Current Version:** 2.2.0  
**Last Updated:** 2025-10-26
