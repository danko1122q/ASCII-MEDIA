# Quick Start Guide
## ASCII-MEDIA v3.0.0

Fast-track guide to get started with ASCII-MEDIA on Linux & macOS.

**⚠️ Windows is no longer supported as of v3.0.0**

---

## Installation

### Linux (Debian/Ubuntu)

```bash
# Install prerequisites
sudo apt update
sudo apt install build-essential gcc g++ libopencv-dev ffmpeg libavcodec-dev libavformat-dev libswscale-dev

# Build C/C++ components (images & GIFs)
make release

# Build Rust components (video & webcam) - optional
cargo build --release --features full

# Install system-wide
sudo make install

# Run
./ascii sample-images/puffin.jpg -D 5 --enhanced-palette
```

### macOS

```bash
# Install prerequisites
brew install gcc opencv ffmpeg

# Build C/C++ components
make release

# Build Rust components - optional
cargo build --release --features full

# Run
./ascii sample-images/puffin.jpg -D 5 --enhanced-palette
```

---

## Prerequisites

### Debian/Ubuntu

```bash
sudo apt install build-essential
```

### Fedora/RHEL

```bash
sudo dnf groupinstall "Development Tools"
```

### Arch Linux

```bash
sudo pacman -S base-devel
```

### macOS

```bash
xcode-select --install
```

### Video/Webcam Features (Optional)

For video playback, webcam streaming, and YouTube support:

```bash
# Linux - install OpenCV and FFmpeg
sudo apt install libopencv-dev ffmpeg libavcodec-dev libavformat-dev

# macOS
brew install opencv ffmpeg

# Build with video/webcam features
cargo build --release --features full
```

---

## Usage Examples

### Dimension Presets

```bash
# Quick preview (Tiny - 40×15) - 4:1 aspect ratio
./ascii image.jpg -D 1

# Default size (Small - 64×24) - Proper proportions
./ascii image.jpg -D 2

# Standard detail (Medium - 100×37) - Recommended
./ascii image.jpg -D 3

# High detail (Large - 150×56) - No distortion
./ascii image.jpg -D 4

# Very high detail (XLarge - 200×75) - Best for braille
./ascii image.jpg -D 5 --braille

# Maximum detail (XXLarge - 250×93) - Proportional
./ascii image.jpg -D 6
```

### Custom Dimensions

```bash
# Specify exact width and height (pixel-perfect accuracy)
./ascii image.jpg -mw 120 -mh 90

# Width only (height auto-calculated to preserve aspect ratio)
./ascii image.jpg -mw 150

# Height only (width auto-calculated)
./ascii image.jpg -mh 100

# Override preset with custom dimensions
./ascii image.jpg -D 3 -mw 120 -mh 80
```

### Color Modes

```bash
# Full 24-bit color (default)
./ascii image.jpg -D 3

# Black and white (grayscale)
./ascii image.jpg -D 3 --grayscale

# 8-color retro palette
./ascii image.jpg -D 3 --retro-colors
```

### Effects

```bash
# Sharpening for enhanced clarity
./ascii image.jpg -D 3 --sharpen 1.2

# Edge detection
./ascii image.jpg -D 3 -et 2.0

# Combined sharpening and edge detection
./ascii image.jpg -D 4 --sharpen 1.0 -et 2.5

# Braille mode for double resolution
./ascii image.jpg -D 5 --braille --sharpen 0.8
```

### GIF Animations

```bash
# Display first frame
./ascii animation.gif -D 3

# Ultra-smooth animation playback (60+ FPS support)
./ascii animation.gif -D 2 --animate

# Animated with grayscale effect
./ascii animation.gif -D 2 --animate --grayscale

# High-detail animated GIF with sharpening
./ascii animation.gif -D 3 --animate --sharpen 1.0
```

**New in v2.3.0:** 
- **4:1 Aspect Ratio**: All presets now use corrected heights to prevent vertical stretching
- **GIF animations** now feature professional-grade smoothness with pre-rendered frames and microsecond-accurate timing
- **Silent playback** - no status messages during animation
- **Hybrid C/C++** architecture for future enhancements

---

## Performance Tips

1. **Preset 3 (Medium)** is optimal for most images
2. **Preset 5 (XLarge)** works best with braille mode
3. **Sharpening range 0.8-1.5** provides good enhancement without artifacts
4. **Edge threshold 2.0-3.0** for subtle edges (portraits)
5. **Edge threshold 1.0-1.5** for strong edges (architecture)
6. **Smaller font size** in terminal shows more detail
7. **Dark backgrounds** generally produce better results
8. **Use preset 2-3 for GIF animation** to maintain smooth playback

---

## Common Issues

### "gcc: command not found"

Install build tools for your platform (see Prerequisites section above).

### "undefined reference to 'sqrt'"

Use provided makefile instead of manual compilation.

### Permission denied on install

Use `sudo` on Linux/macOS or run Command Prompt as Administrator on Windows.

### No colors

Terminal doesn't support true color. Try: `./ascii image.jpg -D 3 --retro-colors`

### Braille shows boxes

Terminal lacks UTF-8 support. Set locale: `export LANG=en_US.UTF-8`

### Output too large

Start with preset 3, then adjust. Avoid preset 6 unless you have a large terminal.

---

## Next Steps

For detailed documentation, see:
- **README.md** - Complete feature documentation
- **TECHNICAL.md** - Algorithm details and implementation
- **CHANGELOG.md** - Version history
- **TESTING_REPORT.md** - Test results

---

**Version:** 2.3.0  
**License:** MIT
