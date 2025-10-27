# Changelog

All notable changes to ASCII-MEDIA are documented in this file.

---

## [3.0.0] - 2025-01-27

### 🎉 MAJOR RELEASE - Complete Rewrite for Linux/macOS

**Breaking Changes:**
- Dropped Windows support - Linux and macOS only
- Updated to ITU-R BT.601 luminance calculation
- Minimum GCC 9.0+ or Clang 10.0+

### 🚀 Major New Features

**Advanced Terminal Integration:**
- **Dynamic Terminal Resize Detection** - SIGWINCH handler for real-time terminal size changes
- **Graceful Shutdown** - SIGINT handler with proper cleanup message: "[+] ASCII-MEDIA terminated safely"
- **Smart Aspect Ratio Lock** - Progressive correction with ±3% tolerance prevents distortion
- **Enhanced Character Palettes** - Three modes:
  - Simple (12 chars) - Fast, compatible
  - Standard (40 chars) - Balanced performance/quality  
  - Enhanced (70+ chars) - Maximum detail with `--enhanced-palette`
- **Debug Mode** - `--debug` flag shows real-time stats: FPS, terminal size, frame dimensions, palette mode

**Image Quality Improvements:**
- **BT.601 Luminance** - Changed from BT.709 to BT.601 standard (0.299R + 0.587G + 0.114B)
- **Gamma Compensation** - Multi-stage luminance with L_gamma = (L)^(1/2.2) for better gradation
- **Adaptive Aspect Ratio** - Automatic fit with smart lock prevents >3% deviation
- **Double Buffering for GIF** - Full frame buffer construction before display eliminates flicker

**Full Video/Webcam Integration (tplay):**
- **Video Playback** - MP4, AVI, MKV, WEBM support via OpenCV + FFmpeg
- **Webcam Streaming** - Live /dev/video* device support
- **YouTube Integration** - Direct URL playback with yt-dlp
- **Audio Sync** - MPV or Rodio backends for synchronized audio
- **Adaptive FPS** - Auto-adjusts frame rate based on system performance

### 🔧 Technical Enhancements

**Build System:**
- Updated Cargo.toml with full tplay dependencies (libwebp-sys, either, num, libmpv)
- Feature flags: `video`, `webcam`, `youtube`, `audio`, `rodio_audio`, `mpv_0_34`, `mpv_0_35`, `full`
- Default feature: `rodio_audio` for maximum compatibility
- Removed all Windows-specific build scripts and MSVC support

**Core Algorithm Updates:**
- ITU-R BT.601 luminance calculation replaces BT.709
- Smart aspect ratio lock with progressive correction algorithm
- Enhanced character palette auto-selection based on performance
- Double buffering implementation for GIF animations
- Memory pre-allocation pool for smooth GIF playback

**Signal Handling:**
- SIGWINCH for terminal resize detection and recalculation
- SIGINT for clean shutdown with resource cleanup
- Thread-safe signal handling with volatile sig_atomic_t

**New Command-Line Options:**
```bash
--enhanced-palette    Use 70+ character precision palette
--debug               Show real-time stats (FPS, size, etc)
```

### 📚 Documentation

**Complete Rewrite:**
- README.md - Linux/macOS focus, video/webcam features, updated examples
- QUICK_START.md - Platform-specific install, video usage examples
- TECHNICAL.md - BT.601 algorithm, SIGWINCH implementation, double buffering details
- All Windows references removed from documentation

### 🐛 Bug Fixes

- Fixed aspect ratio calculation precision issues
- Improved terminal size detection edge cases
- Memory leak fixes in GIF animation cleanup
- Thread-safe signal handler implementation

### 🗑️ Removed

- Windows support (build.bat, install.bat, MinGW references)
- MSVC compiler support in CMake
- Windows-specific terminal detection code
- All Windows-related documentation

### ⚙️ Dependencies

**New Dependencies:**
- libwebp-sys 0.11 - WEBP image format support
- num 0.4 - Numerical operations
- either 1.8 - Type utilities
- libmpv 2.0.1 (optional) - Audio backend for MPV 0.34
- libmpv-sirno 2.0.2-fork.1 (optional) - Audio for MPV 0.35+

**Updated Dependencies:**
- clap 4.2 - Command line parsing
- crossterm 0.28 - Terminal control
- opencv 0.94.2 - Video processing
- ffmpeg-next 7.1.0 - Video codec support
- rodio 0.20.1 - Default audio backend

### 📊 Performance

- GIF animation: 60+ FPS with pre-rendering
- Video playback: Adaptive FPS up to native frame rate
- Memory usage: Pre-allocated pools reduce fragmentation
- Terminal rendering: Double buffering eliminates flicker
- Smart aspect ratio: <1% performance impact

### 🔜 Known Limitations

- Video/webcam features require build with feature flags
- MPV audio requires system libmpv installation
- OpenCV required for video/webcam (Linux packages available)

---

## [2.3.0] - 2025-10-27

### Major Improvements

- **Hybrid C/C++/Rust Architecture**: Foundation for future multimedia features
- **Aspect Ratio Correction**: True 4:1 ratio for proportional output
- **Ultra-Smooth GIF Animation**: Professional-grade 60+ FPS playback

[Previous changelog entries omitted for brevity]

---

## Version Numbering

This project follows semantic versioning (MAJOR.MINOR.PATCH):
- **MAJOR**: Incompatible API changes, platform support changes
- **MINOR**: New functionality, backward compatible
- **PATCH**: Bug fixes, backward compatible

---

## License

MIT License - see LICENSE file for details.

Copyright (c) 2025 danko12
