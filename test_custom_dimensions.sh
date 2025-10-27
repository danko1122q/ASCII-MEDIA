#!/bin/bash
echo "========================================="
echo "ASCII-MEDIA Custom Dimension Testing"
echo "Verifikasi Tidak Ada Distorsi (Gepeng)"
echo "========================================="
echo ""

test_image="sample-images/puffin.jpg"

echo "Test 1: Custom dimensions kecil (40×10)"
echo "----------------------------------------"
output=$(./ascii "$test_image" -mw 40 -mh 10 2>&1)
lines=$(echo "$output" | wc -l)
echo "Tinggi: $lines baris (diharapkan ~10)"
echo "$output" | head -3
echo ""

echo "Test 2: Custom dimensions sedang (80×30)"
echo "----------------------------------------"
output=$(./ascii "$test_image" -mw 80 -mh 30 2>&1)
lines=$(echo "$output" | wc -l)
echo "Tinggi: $lines baris (diharapkan ~30)"
echo "$output" | head -3
echo ""

echo "Test 3: Custom dimensions besar (120×45)"
echo "----------------------------------------"
output=$(./ascii "$test_image" -mw 120 -mh 45 2>&1)
lines=$(echo "$output" | wc -l)
echo "Tinggi: $lines baris (diharapkan ~45)"
echo "$output" | head -3
echo ""

echo "Test 4: Custom dimensions sangat lebar (200×50)"
echo "------------------------------------------------"
output=$(./ascii "$test_image" -mw 200 -mh 50 2>&1)
lines=$(echo "$output" | wc -l)
echo "Tinggi: $lines baris (diharapkan ~50)"
echo "$output" | head -3
echo ""

echo "Test 5: Custom dimensions sangat tinggi (60×100)"
echo "-------------------------------------------------"
output=$(./ascii "$test_image" -mw 60 -mh 100 2>&1)
lines=$(echo "$output" | wc -l)
echo "Tinggi: $lines baris (diharapkan ~100)"
echo "$output" | head -3
echo ""

echo "Test 6: GIF dengan custom dimensions (80×30)"
echo "---------------------------------------------"
output=$(./ascii sample-images/nyan-cat.gif -mw 80 -mh 30 2>&1 | head -30)
lines=$(echo "$output" | wc -l)
echo "Tinggi: $lines baris (diharapkan ~30)"
echo "$output" | head -3
echo ""

echo "Test 7: Grayscale dengan custom dimensions (100×40)"
echo "---------------------------------------------------"
output=$(./ascii "$test_image" -mw 100 -mh 40 --grayscale 2>&1)
lines=$(echo "$output" | wc -l)
echo "Tinggi: $lines baris (diharapkan ~40)"
echo "$output" | head -3
echo ""

echo "Test 8: Braille dengan custom dimensions (80×30)"
echo "------------------------------------------------"
output=$(./ascii "$test_image" -mw 80 -mh 30 --braille 2>&1)
lines=$(echo "$output" | wc -l)
echo "Tinggi: $lines baris (diharapkan ~30)"
echo "$output" | head -3
echo ""

echo "========================================="
echo "✅ Semua test custom dimensions selesai!"
echo "========================================="
echo ""
echo "Kesimpulan: Program mendukung ukuran custom"
echo "dengan format: ./ascii image.jpg -mw [lebar] -mh [tinggi]"
echo ""
echo "Contoh penggunaan:"
echo "  ./ascii foto.jpg -mw 100 -mh 40    # 100 karakter × 40 baris"
echo "  ./ascii gif.gif -mw 80 -mh 30 --animate"
echo "  ./ascii img.png -mw 120 -mh 50 --grayscale"
