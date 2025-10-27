#!/bin/bash
echo "==================================="
echo "ASCII-MEDIA Comprehensive Test Suite"
echo "Version 2.3.0"
echo "==================================="
echo ""

fail_count=0
pass_count=0

test_feature() {
    echo -n "Testing: $1... "
    if eval "$2" > /dev/null 2>&1; then
        echo "✓ PASS"
        ((pass_count++))
    else
        echo "✗ FAIL"
        ((fail_count++))
    fi
}

echo "1. Build System Tests"
echo "---------------------"
test_feature "Binary exists" "[ -f ./ascii ]"
test_feature "Binary executable" "[ -x ./ascii ]"
test_feature "Help command" "./ascii --help"
test_feature "Version command" "./ascii --version"
echo ""

echo "2. Image Format Tests"
echo "---------------------"
test_feature "JPEG loading" "./ascii sample-images/puffin.jpg -D 1"
test_feature "GIF loading" "./ascii sample-images/nyan-cat.gif -D 1"
echo ""

echo "3. Dimension Preset Tests"
echo "-------------------------"
for i in 1 2 3 4 5 6; do
    test_feature "Preset $i" "./ascii sample-images/puffin.jpg -D $i"
done
echo ""

echo "4. Feature Tests"
echo "----------------"
test_feature "Grayscale mode" "./ascii sample-images/puffin.jpg -D 2 --grayscale"
test_feature "Sharpening" "./ascii sample-images/puffin.jpg -D 2 --sharpen 1.0"
test_feature "Edge detection" "./ascii sample-images/puffin.jpg -D 2 -et 2.0"
test_feature "Braille mode" "./ascii sample-images/puffin.jpg -D 2 --braille"
test_feature "Retro colors" "./ascii sample-images/puffin.jpg -D 2 --retro-colors"
test_feature "Custom dimensions" "./ascii sample-images/puffin.jpg -mw 80 -mh 30"
echo ""

echo "5. GIF Animation Test"
echo "---------------------"
timeout 2 ./ascii sample-images/nyan-cat.gif -D 2 --animate > /tmp/gif_test.txt 2>&1
if ! grep -q "Playing GIF" /tmp/gif_test.txt && ! grep -q "Press Ctrl+C" /tmp/gif_test.txt; then
    echo "✓ PASS - Silent GIF playback (no status messages)"
    ((pass_count++))
else
    echo "✗ FAIL - GIF status messages still present"
    ((fail_count++))
fi
echo ""

echo "6. Error Handling Tests"
echo "-----------------------"
if ./ascii nonexistent.jpg 2>&1 | grep -q "Error"; then
    echo "✓ PASS - File not found error"
    ((pass_count++))
else
    echo "✗ FAIL - Error handling"
    ((fail_count++))
fi
echo ""

echo "==================================="
echo "Test Results Summary"
echo "==================================="
echo "Total tests: $((pass_count + fail_count))"
echo "Passed: $pass_count ✓"
echo "Failed: $fail_count ✗"
echo ""

if [ $fail_count -eq 0 ]; then
    echo "🎉 ALL TESTS PASSED! 🎉"
    echo "Project is stable and ready for production!"
    exit 0
else
    echo "⚠️  Some tests failed. Please review."
    exit 1
fi
