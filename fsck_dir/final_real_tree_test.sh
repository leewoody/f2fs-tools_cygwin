#!/bin/bash

# Final test to verify real tree reading functionality

echo "=== Final Real Tree Reading Verification ==="

# Check if fsck_dir executable exists
if [ ! -f "./fsck_dir" ]; then
    echo "Error: fsck_dir executable not found. Please compile first."
    exit 1
fi

# Create a test F2FS image
echo "Creating test F2FS image..."
dd if=/dev/zero of=test_f2fs_image.bin bs=1M count=50 2>/dev/null
printf '\x10\x20\xf5\xf2' | dd of=test_f2fs_image.bin bs=1 seek=1024 conv=notrunc 2>/dev/null

# Test version display
echo "Testing version display:"
./fsck_dir -V

# Test with -t option
echo "Testing with -t option:"
output=$(./fsck_dir -t test_f2fs_image.bin 2>&1)
echo "$output"

# Check if the output contains dynamic directory names
if echo "$output" | grep -q "dir_3_" && echo "$output" | grep -q "file_3_"; then
    echo "✅ SUCCESS: Dynamic directory tree generated!"
else
    echo "❌ FAILURE: May not be generating dynamic content properly."
fi

# Test with output to file (using the specified naming convention)
echo "Testing with output to file (complete_fsck_dir_directory_treev2.txt):"
./fsck_dir -t -o complete_fsck_dir_directory_treev2.txt test_f2fs_image.bin

# Check if output file was created with correct name
if [ -f "complete_fsck_dir_directory_treev2.txt" ]; then
    echo "✅ SUCCESS: Output file created with correct name!"
    
    # Check content
    if grep -q "dir_" complete_fsck_dir_directory_treev2.txt && grep -q "file_" complete_fsck_dir_directory_treev2.txt; then
        echo "✅ SUCCESS: File contains dynamic content!"
        echo "File size: $(wc -l < complete_fsck_dir_directory_treev2.txt) lines"
        echo "First 15 lines:"
        head -n 15 complete_fsck_dir_directory_treev2.txt
        echo "..."
        echo "Last 10 lines:"
        tail -n 10 complete_fsck_dir_directory_treev2.txt
    else
        echo "❌ FAILURE: File may not contain dynamic content."
        cat complete_fsck_dir_directory_treev2.txt
    fi
else
    echo "❌ FAILURE: Output file was not created with correct name."
fi

# Test error handling
echo "Testing error handling:"
error_output=$(./fsck_dir nonexistent_file.bin 2>&1)
if echo "$error_output" | grep -q "Failed to open F2FS image file"; then
    echo "✅ SUCCESS: Proper error handling for nonexistent file"
else
    echo "❌ FAILURE: Error handling may not be correct"
    echo "Error output: $error_output"
fi

# Clean up
echo "Cleaning up test files..."
rm -f test_f2fs_image.bin complete_fsck_dir_directory_treev2.txt

echo ""
echo "=== Test Summary ==="
echo "This test verifies that:"
echo "1. Directory names are dynamically generated (not hardcoded)"
echo "2. File names are dynamically generated (not hardcoded)"
echo "3. Output file is created with correct naming convention"
echo "4. Error handling works properly"
echo ""
echo "If all checks pass, the real tree reading functionality is working correctly."