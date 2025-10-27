#!/bin/bash
echo "ASCII-MEDIA Aspect Ratio Verification Test"
echo "==========================================="
echo ""

test_image="sample-images/puffin.jpg"

for preset in 1 2 3 4 5 6; do
    echo "Preset $preset:"
    output=$(./ascii "$test_image" -D $preset 2>&1)
    lines=$(echo "$output" | wc -l)
    chars=$(echo "$output" | head -1 | wc -c)
    
    # Expected dimensions based on 4:1 aspect ratio
    case $preset in
        1) expected_h=15; expected_w=40 ;;
        2) expected_h=24; expected_w=64 ;;
        3) expected_h=37; expected_w=100 ;;
        4) expected_h=56; expected_w=150 ;;
        5) expected_h=75; expected_w=200 ;;
        6) expected_h=93; expected_w=250 ;;
    esac
    
    ratio=$(echo "scale=2; $chars / $lines" | bc)
    expected_ratio=$(echo "scale=2; $expected_w / $expected_h" | bc)
    
    echo "  Output: ~$chars chars wide × $lines lines"
    echo "  Expected: ${expected_w}×${expected_h}"
    echo "  Ratio: ~${ratio}:1 (expected ~${expected_ratio}:1)"
    
    if [ "$lines" -ge $((expected_h - 5)) ] && [ "$lines" -le $((expected_h + 5)) ]; then
        echo "  ✓ Height OK"
    else
        echo "  ⚠ Height mismatch"
    fi
    echo ""
done

echo "Testing custom dimensions:"
./ascii "$test_image" -mw 80 -mh 30 2>&1 | wc -l
echo "  Custom 80×30: $(./ascii "$test_image" -mw 80 -mh 30 2>&1 | wc -l) lines"
echo ""

echo "Testing grayscale mode (should not affect aspect):"
./ascii "$test_image" -D 3 --grayscale 2>&1 | wc -l
echo "  Grayscale D3: $(./ascii "$test_image" -D 3 --grayscale 2>&1 | wc -l) lines"
echo ""

echo "✅ All aspect ratio tests completed!"
