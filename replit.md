# ASCII-MEDIA Project Memory

This file tracks project state, user preferences, and architectural decisions for session continuity.

---

## Project Overview

**Name**: ASCII-MEDIA v3.0.0  
**Purpose**: Terminal ASCII art converter for images and GIFs  
**Status**: ✅ Production Ready  
**Last Updated**: 2025-10-28

---

## Critical User Requirements

### Primary Goal (MUST NOT BREAK)
**"Output must NEVER be 'gepeng' (distorted/squashed)"**
- Vertical images MUST stay vertical
- Horizontal images MUST stay horizontal
- Square images MUST stay square
- Aspect ratio deviation tolerance: ±3% maximum

### Core Features (Priority Order)
1. **ASCII Image** - Convert images to ASCII art (WORKING ✅)
2. **ASCII GIF** - Animate GIFs in terminal (WORKING ✅)
3. **ASCII Video** - Video support (PLANNED for v3.1)

### Removed Features
- ❌ Webcam functionality (removed v3.0.0 - not core feature)

---

## Recent Changes

### 2025-10-28 - v3.0.0 Production Release

**Critical Bug Fix:**
- Fixed aspect ratio distortion in `make_resized()` function (src/image.c)
- Issue: Minimum size clamps were applied AFTER aspect ratio calculation
- Solution: Enforce minimums BEFORE scaling to preserve aspect ratio
- Result: Perfect proportions - no more "gepeng" output!

**Code Cleanup:**
- Removed duplicate video code (video_player.cpp, main_video.cpp)
- Removed unused Rust modules: audio/, downloader/, msg/, pipeline/, terminal/, common/
- Removed webcam functionality from rust/src/main.rs
- Removed backup files (main.rs.bak) and temp files
- Removed attached_assets/ folder (prompts not needed in code)

**Documentation:**
- Complete rewrite of README.md (detailed, accurate)
- Updated QUICK_START.md (removed webcam references)
- Updated TECHNICAL.md (aspect ratio algorithm explained)
- Updated CHANGELOG.md (v3.0.0 release notes)
- Updated doc.md (project documentation)
- Updated TESTING_REPORT.md (validation results)
- Created replit.md (this file for memory tracking)

**Validation:**
- ✅ Clean build (no errors, no warnings)
- ✅ LSP diagnostics clean
- ✅ Image rendering perfect
- ✅ GIF animation smooth (60+ FPS)
- ✅ Aspect ratio perfect (no warnings)
- ✅ All tests passing

---

## Architecture Decisions

### Hybrid C/C++/Rust Design

**Why This Architecture:**
- **C Core**: Fast image processing, STB library integration
- **C++ Advanced**: Bilinear interpolation, gamma correction
- **Rust Orchestration**: CLI interface, future video routing

**Component Separation:**
```
ascii (C binary) → Image/GIF processor
  ├── src/image.c - Load, resize, sharpen, GIF handling
  ├── src/print_image.c - Terminal rendering, animation
  └── src/argparse.c - CLI parsing

ascii-media (Rust binary) → Media routing orchestrator
  └── rust/src/main.rs - Route to appropriate processor
```

### Aspect Ratio Correction Formula

**Design Choice: Character Ratio = 2:1**
- Terminal characters are approximately 2x taller than wide
- Formula: `aspect_corrected = (width / height) / character_ratio`
- This compensates for terminal font dimensions

**Progressive Correction with Tolerance:**
- Calculate target dimensions maintaining aspect ratio
- Validate final output aspect vs original image aspect
- Warning if deviation > 3%
- This prevents accumulation of rounding errors

**Implementation Location:**
- Function: `make_resized()` in `src/image.c`
- Lines: ~235-310 (aspect ratio calculation and validation)

---

## Technical Specifications

### Image Processing Pipeline
1. Load image (STB library)
2. Optional: Unsharp mask sharpening
3. **Calculate aspect ratio with character correction** (CRITICAL)
4. Resize maintaining aspect ratio (area averaging)
5. Calculate luminance (ITU-R BT.601)
6. Optional: Edge detection (Sobel)
7. Map to character palette
8. Render with ANSI colors

### GIF Animation Pipeline
1. Load all frames (stbi_load_gif_from_memory)
2. Pre-process frames ONCE:
   - Resize (with aspect correction)
   - Sharpen (on resized frames)
3. Display loop with precise timing
4. Cleanup all frames

### Performance Metrics
- Image processing: <0.3s (preset 3)
- GIF animation: 60+ FPS capable
- Memory: ~5MB for preset 3
- Build time: ~15s release build

---

## User Preferences

### Coding Style
- C code: C99 standard, 4-space indent
- C++ code: C++17, RAII
- Rust code: cargo fmt, idiomatic Rust
- Comments: Explain WHY, not WHAT (code should be self-documenting)

### Documentation Style
- Technical but accessible
- Detailed examples with real use cases
- Performance metrics where relevant
- Clear troubleshooting sections

### Workflow Preferences
- Always validate changes with test images
- Check aspect ratio warnings
- Rebuild and test GIF animation
- Update documentation immediately after code changes

---

## Project Structure

```
ASCII-MEDIA/
├── src/               # C source files
│   ├── main.c
│   ├── argparse.c
│   ├── image.c       # CRITICAL: Aspect ratio fix here
│   └── print_image.c
├── include/          # C headers
├── rust/src/         # Rust orchestration
│   └── main.rs       # Simplified (webcam removed)
├── sample-images/    # Test images
├── build/            # Build artifacts
├── target/           # Rust build output
├── Makefile          # Main build system
├── CMakeLists.txt    # C/C++ compilation
├── Cargo.toml        # Rust dependencies
└── Documentation/
    ├── README.md
    ├── QUICK_START.md
    ├── TECHNICAL.md
    ├── CHANGELOG.md
    ├── doc.md
    ├── TESTING_REPORT.md
    └── replit.md (this file)
```

---

## Important Files to Monitor

**Critical Code:**
- `src/image.c` - Aspect ratio calculation in `make_resized()`
- `src/print_image.c` - GIF animation rendering
- `rust/src/main.rs` - CLI routing

**Build Configuration:**
- `Makefile` - Primary build system
- `CMakeLists.txt` - C/C++ compiler flags
- `Cargo.toml` - Rust dependencies

**Documentation:**
- `README.md` - User-facing complete guide
- `TECHNICAL.md` - Technical implementation details
- `CHANGELOG.md` - Version history

---

## Known Issues & Limitations

### Current Limitations
- Video support: Planned for v3.1 (Rust pipeline)
- Webcam: Removed in v3.0.0 (not core feature)
- Windows: Not tested extensively (Linux/macOS primary)

### No Known Bugs
- ✅ Aspect ratio: FIXED in v3.0.0
- ✅ GIF animation: Working perfectly
- ✅ Build system: Clean compilation
- ✅ Memory leaks: None detected

---

## Testing Checklist

**Before Marking Complete:**
- [x] Build succeeds without errors
- [x] LSP diagnostics clean
- [x] Test with vertical image (stays vertical)
- [x] Test with horizontal image (stays horizontal)
- [x] Test with square image (stays square)
- [x] Test GIF animation (smooth playback)
- [x] No aspect ratio warnings
- [x] All documentation updated

**Sample Test Commands:**
```bash
# Build
make clean && make release

# Test images
./ascii sample-images/puffin.jpg -D 3
./ascii sample-images/penguin.jpg -D 3 --sharpen 1.0
./ascii sample-images/lion.jpg -D 3 --grayscale

# Test GIF
./ascii sample-images/nyan-cat.gif -D 2 --animate

# Test aspect ratio
./ascii portrait.jpg -D 3  # Should be vertical
./ascii landscape.jpg -D 3  # Should be horizontal
./ascii square.jpg -D 3     # Should be square
```

---

## Future Development Notes

### v3.1 Planned Features
- Video file support (MP4, AVI, MKV)
  - Use existing Rust pipeline with OpenCV
  - Implement audio sync
  - Add frame extraction and ASCII conversion
- Custom color palette import
- Performance optimizations for large files

### v3.2 Ideas
- Optional webcam support (feature flag)
- YouTube video download & play
- WebP animation support
- HTML/SVG export

### v4.0 Vision
- GPU acceleration for real-time video
- Network streaming support
- Interactive controls (pause, seek, etc)

---

## Session Continuity

**Current State:**
- Version: 3.0.0
- Status: Production Ready
- Last Task: Complete documentation update
- Next Steps: Final architect review

**If Session Resets:**
1. Read this file first
2. Verify critical requirement: NO "gepeng" output
3. Check `src/image.c` make_resized() function
4. Test aspect ratio with sample images
5. Review documentation accuracy

---

**Last Updated**: 2025-10-28  
**Project Status**: ✅ Production Ready  
**Aspect Ratio**: ✅ Perfect (No Distortion)
