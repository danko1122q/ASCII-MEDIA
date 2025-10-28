# Quick Start Guide
## ASCII-MEDIA v3.0.0

Fast-track guide to get started with ASCII-MEDIA for converting images and GIFs into stunning terminal ASCII art.

---

## Installation

### Linux (Debian/Ubuntu)

```bash
# Install prerequisites
sudo apt update
sudo apt install build-essential gcc g++ cmake

# Build project
make release

# Install system-wide (optional)
sudo make install

# Run
./ascii sample-images/puffin.jpg -D 3
```

### Linux (Fedora/RHEL)

```bash
# Install prerequisites
sudo dnf groupinstall "Development Tools"

# Build and run
make release
./ascii sample-images/puffin.jpg -D 3
```

### Linux (Arch)

```bash
# Install prerequisites
sudo pacman -S base-devel cmake

# Build and run
make release
./ascii sample-images/puffin.jpg -D 3
```

### macOS

```bash
# Install prerequisites
xcode-select --install

# Build and run
make release
./ascii sample-images/puffin.jpg -D 3
```

---

## Quick Examples

### Basic Image Display

```bash
# Standard rendering with preset 3 (recommended)
./ascii sample-images/puffin.jpg -D 3

# Auto-detect terminal size
./ascii sample-images/mountain.jpg

# Custom dimensions
./ascii sample-images/lion.jpg -mw 120 -mh 45
```

### Color Modes

```bash
# Full 24-bit truecolor (default)
./ascii sample-images/puffin.jpg -D 3

# Grayscale (black & white)
./ascii sample-images/puffin.jpg -D 3 --grayscale

# Retro 8-color palette
./ascii sample-images/puffin.jpg -D 3 --retro-colors
```

### Image Enhancement

```bash
# Sharpening for enhanced clarity
./ascii sample-images/penguin.jpg -D 3 --sharpen 1.2

# Edge detection
./ascii sample-images/kontrast.jpg -D 4 -et 2.0

# Combined: sharpen + edge detection
./ascii sample-images/mountain.jpg -D 4 --sharpen 1.0 -et 2.5
```

### GIF Animations

```bash
# Display first frame only
./ascii sample-images/nyan-cat.gif -D 3

# Ultra-smooth animation playback (60+ FPS)
./ascii sample-images/nyan-cat.gif -D 2 --animate

# Animated with grayscale effect
./ascii sample-images/nyan-cat.gif -D 2 --animate --grayscale

# High-detail animated GIF with sharpening
./ascii sample-images/nyan-cat.gif -D 3 --animate --sharpen 0.5
```

### Braille Mode (High Detail)

```bash
# Standard braille
./ascii sample-images/penguin.jpg -D 5 --braille

# Braille with sharpening
./ascii sample-images/lion.jpg -D 5 --braille --sharpen 0.8

# Grayscale braille
./ascii sample-images/puffin.jpg -D 5 --braille --grayscale
```

---

## Dimension Presets

Quick access to pre-defined sizes optimized for terminal display:

| Preset | Command | Dimensions | Characters | Use Case |
|--------|---------|------------|------------|----------|
| 1 | `-D 1` | 40×15 | 600 | Quick preview, small terminal |
| 2 | `-D 2` | 64×24 | 1,536 | Default, compact view |
| 3 | `-D 3` | 100×37 | 3,700 | **Recommended**, standard detail |
| 4 | `-D 4` | 150×56 | 8,400 | High detail, large terminal |
| 5 | `-D 5` | 200×75 | 15,000 | Very high detail, optimal for braille |
| 6 | `-D 6` | 250×93 | 23,250 | Maximum detail, ultra-wide screen |

**Note**: All presets automatically maintain perfect aspect ratio to prevent distortion.

---

## Performance Tips

1. **Preset 3 (Medium)** - Optimal for most images
2. **Preset 5 (XLarge)** - Best with braille mode
3. **Sharpening 0.8-1.5** - Good enhancement without artifacts
4. **Edge threshold 2.0-3.0** - Subtle edges (portraits)
5. **Edge threshold 1.0-1.5** - Strong edges (architecture)
6. **Smaller presets (2-3)** - Better for smooth GIF animation
7. **Smaller font size** - Shows more detail in terminal
8. **Dark backgrounds** - Generally produce better contrast

---

## Common Issues

### "gcc: command not found"

Install build tools for your platform:
```bash
# Debian/Ubuntu
sudo apt install build-essential

# Fedora/RHEL
sudo dnf groupinstall "Development Tools"

# Arch
sudo pacman -S base-devel

# macOS
xcode-select --install
```

### "undefined reference to 'sqrt'"

Always use the provided Makefile (includes `-lm` math library flag):
```bash
make release
```

### Permission denied on install

Use `sudo` for system-wide installation:
```bash
sudo make install
```

### No colors displayed

Terminal doesn't support true color. Try retro mode:
```bash
./ascii image.jpg -D 3 --retro-colors
```

### Braille shows boxes

Terminal lacks UTF-8 support. Set locale:
```bash
export LANG=en_US.UTF-8
export LC_ALL=en_US.UTF-8
```

Install font with braille support (DejaVu Sans Mono, Fira Code).

### Output too large/small

Start with preset 3, then adjust:
```bash
# If too large
./ascii image.jpg -D 2

# If too small
./ascii image.jpg -D 4

# Or use custom dimensions
./ascii image.jpg -mw 100 -mh 40
```

### GIF animation stuttering

Use smaller preset for smoother playback:
```bash
./ascii animation.gif --animate -D 2
```

### Aspect ratio warning appears

If deviation < 5%: Acceptable, slight distortion tolerated  
If deviation > 5%: Adjust character ratio:
```bash
./ascii image.jpg -D 3 -cr 1.8
# or
./ascii image.jpg -D 3 -cr 2.2
```

---

## Next Steps

For detailed documentation, see:
- **README.md** - Complete feature documentation
- **TECHNICAL.md** - Algorithm details and implementation
- **CHANGELOG.md** - Version history

---

**Version:** 3.0.0  
**License:** MIT  
**Platform:** Linux & macOS
