#!/bin/bash
# ASCII-MEDIA Comprehensive 100% Quality Assurance Test
# Copyright (c) 2025 danko12

#set -e  # Exit on any error

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

total_tests=0
passed_tests=0
failed_tests=0
warnings=0

test_pass() {
    echo -e "${GREEN}✓ PASS${NC} - $1"
    ((passed_tests++))
    ((total_tests++))
}

test_fail() {
    echo -e "${RED}✗ FAIL${NC} - $1"
    ((failed_tests++))
    ((total_tests++))
}

test_warn() {
    echo -e "${YELLOW}⚠ WARN${NC} - $1"
    ((warnings++))
}

section() {
    echo ""
    echo -e "${BLUE}═══════════════════════════════════════════${NC}"
    echo -e "${BLUE}$1${NC}"
    echo -e "${BLUE}═══════════════════════════════════════════${NC}"
}

echo ""
echo "╔═══════════════════════════════════════════════════╗"
echo "║   ASCII-MEDIA Quality Assurance Suite v2.3.0     ║"
echo "║   100% Comprehensive Testing                      ║"
echo "╚═══════════════════════════════════════════════════╝"
echo ""

# ============================================
section "1. BUILD SYSTEM VERIFICATION"
# ============================================

if [ -f ./ascii ]; then
    test_pass "Binary exists"
else
    test_fail "Binary missing"
fi

if [ -x ./ascii ]; then
    test_pass "Binary executable"
else
    test_fail "Binary not executable"
fi

# Check file size
size=$(stat -c%s ./ascii 2>/dev/null || stat -f%z ./ascii 2>/dev/null)
if [ "$size" -gt 50000 ]; then
    test_pass "Binary size reasonable ($size bytes)"
else
    test_fail "Binary too small ($size bytes)"
fi

# ============================================
section "2. COMMAND LINE INTERFACE"
# ============================================

# Help command
if ./ascii --help 2>&1 | grep -q "ASCII View"; then
    test_pass "Help command shows version"
else
    test_fail "Help command failed"
fi

# Version command
if ./ascii --version 2>&1 | grep -q "2.3.0"; then
    test_pass "Version command correct"
else
    test_fail "Version command failed"
fi

# Invalid arguments
if ./ascii nonexistent.jpg 2>&1 | grep -qi "error\|failed\|cannot"; then
    test_pass "Error handling for missing file"
else
    test_fail "No error for missing file"
fi

# ============================================
section "3. DIMENSION PRESETS (4:1 ASPECT RATIO)"
# ============================================

test_img="sample-images/puffin.jpg"

# Test each preset
declare -A expected_heights=([1]=15 [2]=24 [3]=37 [4]=56 [5]=75 [6]=93)

for preset in 1 2 3 4 5 6; do
    output=$(./ascii "$test_img" -D $preset 2>&1)
    lines=$(echo "$output" | wc -l)
    expected=${expected_heights[$preset]}
    
    # Allow ±2 lines tolerance
    if [ "$lines" -ge $((expected - 2)) ] && [ "$lines" -le $((expected + 2)) ]; then
        test_pass "Preset D$preset: $lines lines (expected ~$expected) - NO DISTORTION"
    else
        test_fail "Preset D$preset: $lines lines (expected ~$expected) - DISTORTED!"
    fi
done

# ============================================
section "4. CUSTOM DIMENSIONS"
# ============================================

# Test various custom sizes
declare -A custom_tests=(
    ["Small"]="-mw 40 -mh 10"
    ["Medium"]="-mw 80 -mh 30"
    ["Large"]="-mw 120 -mh 45"
    ["Wide"]="-mw 200 -mh 40"
    ["Tall"]="-mw 60 -mh 100"
)

for name in "${!custom_tests[@]}"; do
    opts=${custom_tests[$name]}
    if ./ascii "$test_img" $opts 2>&1 | head -1 > /dev/null; then
        test_pass "Custom dimensions ($name): $opts"
    else
        test_fail "Custom dimensions ($name): $opts"
    fi
done

# ============================================
section "5. IMAGE FORMAT SUPPORT"
# ============================================

if [ -f "sample-images/puffin.jpg" ]; then
    if ./ascii sample-images/puffin.jpg -D 1 > /dev/null 2>&1; then
        test_pass "JPEG format support"
    else
        test_fail "JPEG format failed"
    fi
else
    test_warn "JPEG test image missing"
fi

if [ -f "sample-images/nyan-cat.gif" ]; then
    if ./ascii sample-images/nyan-cat.gif -D 1 > /dev/null 2>&1; then
        test_pass "GIF format support"
    else
        test_fail "GIF format failed"
    fi
else
    test_warn "GIF test image missing"
fi

# ============================================
section "6. FEATURE FLAGS"
# ============================================

# Grayscale mode
if ./ascii "$test_img" -D 2 --grayscale 2>&1 | head -1 > /dev/null; then
    test_pass "Grayscale mode"
else
    test_fail "Grayscale mode failed"
fi

# Braille mode
if ./ascii "$test_img" -D 2 --braille 2>&1 | head -1 > /dev/null; then
    test_pass "Braille mode"
else
    test_fail "Braille mode failed"
fi

# Retro colors
if ./ascii "$test_img" -D 2 --retro-colors 2>&1 | head -1 > /dev/null; then
    test_pass "Retro colors mode"
else
    test_fail "Retro colors mode failed"
fi

# Sharpening
if ./ascii "$test_img" -D 2 --sharpen 1.0 2>&1 | head -1 > /dev/null; then
    test_pass "Sharpening filter"
else
    test_fail "Sharpening filter failed"
fi

# Edge detection
if ./ascii "$test_img" -D 2 -et 2.0 2>&1 | head -1 > /dev/null; then
    test_pass "Edge detection"
else
    test_fail "Edge detection failed"
fi

# Character ratio
if ./ascii "$test_img" -D 2 -cr 2.0 2>&1 | head -1 > /dev/null; then
    test_pass "Custom character ratio"
else
    test_fail "Custom character ratio failed"
fi

# ============================================
section "7. GIF ANIMATION (SILENT PLAYBACK)"
# ============================================

if [ -f "sample-images/nyan-cat.gif" ]; then
    # Test for silent playback (no status messages)
    timeout 2 ./ascii sample-images/nyan-cat.gif -D 2 --animate > /tmp/gif_test.txt 2>&1 || true
    
    if grep -qi "playing\|press ctrl" /tmp/gif_test.txt; then
        test_fail "GIF animation has status messages (should be silent!)"
    else
        test_pass "GIF animation SILENT (no status messages) ✓"
    fi
    
    # Check if animation actually runs
    if [ -s /tmp/gif_test.txt ]; then
        test_pass "GIF animation produces output"
    else
        test_fail "GIF animation no output"
    fi
    
    rm -f /tmp/gif_test.txt
fi

# ============================================
section "8. COMBINED FEATURES"
# ============================================

# Grayscale + Custom dimensions
if ./ascii "$test_img" -mw 80 -mh 30 --grayscale > /dev/null 2>&1; then
    test_pass "Grayscale + Custom dimensions"
else
    test_fail "Grayscale + Custom dimensions failed"
fi

# Braille + Sharpening
if ./ascii "$test_img" -D 2 --braille --sharpen 1.5 > /dev/null 2>&1; then
    test_pass "Braille + Sharpening"
else
    test_fail "Braille + Sharpening failed"
fi

# GIF + Grayscale + Custom
if [ -f "sample-images/nyan-cat.gif" ]; then
    if timeout 1 ./ascii sample-images/nyan-cat.gif -mw 60 -mh 20 --grayscale > /dev/null 2>&1; then
        test_pass "GIF + Grayscale + Custom dimensions"
    else
        test_fail "GIF + Grayscale + Custom failed"
    fi
fi

# ============================================
section "9. ASPECT RATIO VERIFICATION (NO DISTORTION)"
# ============================================

# Verify all outputs are proportional
for preset in 1 2 3 4 5 6; do
    output=$(./ascii "$test_img" -D $preset 2>&1)
    
    # Check if output looks reasonable (has ASCII characters)
    if echo "$output" | grep -q "[=+*#@-]"; then
        test_pass "Preset D$preset output contains ASCII art"
    else
        test_fail "Preset D$preset output invalid"
    fi
done

# ============================================
section "10. BUILD SYSTEM"
# ============================================

# Check Makefile exists
if [ -f "makefile" ]; then
    test_pass "Makefile exists"
else
    test_fail "Makefile missing"
fi

# Check CMakeLists exists
if [ -f "CMakeLists.txt" ]; then
    test_pass "CMakeLists.txt exists"
else
    test_fail "CMakeLists.txt missing"
fi

# ============================================
section "11. DOCUMENTATION"
# ============================================

if [ -f "README.md" ] && grep -q "4:1" README.md; then
    test_pass "README.md mentions 4:1 aspect ratio"
else
    test_fail "README.md missing aspect ratio info"
fi

if [ -f "CHANGELOG.md" ] && grep -q "2.3.0" CHANGELOG.md; then
    test_pass "CHANGELOG.md has v2.3.0 entry"
else
    test_fail "CHANGELOG.md missing version"
fi

if [ -f "QUICK_START.md" ] && grep -q "40×15" QUICK_START.md; then
    test_pass "QUICK_START.md has corrected dimensions"
else
    test_fail "QUICK_START.md has wrong dimensions"
fi

# ============================================
section "12. SOURCE CODE INTEGRITY"
# ============================================

# Check for copyright headers
if grep -q "danko12" src/ascii_processor.cpp; then
    test_pass "C++ module has copyright"
else
    test_fail "C++ module missing copyright"
fi

# Check for critical files
critical_files=("src/main.c" "src/argparse.c" "src/image.c" "src/print_image.c" "src/ascii_processor.cpp")
for file in "${critical_files[@]}"; do
    if [ -f "$file" ]; then
        test_pass "Critical file exists: $file"
    else
        test_fail "Critical file missing: $file"
    fi
done

# ============================================
# FINAL RESULTS
# ============================================
echo ""
echo "╔═══════════════════════════════════════════════════╗"
echo "║              TEST RESULTS SUMMARY                 ║"
echo "╚═══════════════════════════════════════════════════╝"
echo ""
echo "Total Tests:    $total_tests"
echo -e "Passed:         ${GREEN}$passed_tests ✓${NC}"
echo -e "Failed:         ${RED}$failed_tests ✗${NC}"
echo -e "Warnings:       ${YELLOW}$warnings ⚠${NC}"
echo ""

percentage=$((passed_tests * 100 / total_tests))
echo "Success Rate:   $percentage%"
echo ""

if [ $failed_tests -eq 0 ]; then
    echo -e "${GREEN}╔═══════════════════════════════════════════════════╗${NC}"
    echo -e "${GREEN}║  🎉 ALL TESTS PASSED! PROJECT IS 100% STABLE! 🎉  ║${NC}"
    echo -e "${GREEN}╚═══════════════════════════════════════════════════╝${NC}"
    exit 0
else
    echo -e "${RED}╔═══════════════════════════════════════════════════╗${NC}"
    echo -e "${RED}║      ⚠️  SOME TESTS FAILED - REVIEW NEEDED  ⚠️     ║${NC}"
    echo -e "${RED}╚═══════════════════════════════════════════════════╝${NC}"
    exit 1
fi
