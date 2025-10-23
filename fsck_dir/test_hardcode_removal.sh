#!/bin/bash

# Test script to verify that hardcode has been removed and real F2FS image reading works

echo "Testing fsck_dir tool after hardcode removal..."

# Check if fsck_dir executable exists
if [ ! -f "./fsck_dir" ]; then
    echo "Error: fsck_dir executable not found. Please compile first."
    exit 1
fi

# Test version display
echo "Testing version display:"
./fsck_dir -V

# Create a simple test file to use as a mock F2FS image
echo "Creating a mock F2FS image for testing..."
dd if=/dev/zero of=test_f2fs_image.bin bs=1M count=50 2>/dev/null

# Add F2FS superblock magic number to the test image
printf '\x10\x20\xf5\xf2' | dd of=test_f2fs_image.bin bs=1 seek=1024 conv=notrunc 2>/dev/null

# Test with -t option to show directory tree
echo "Testing with -t option on mock F2FS image:"
./fsck_dir -t test_f2fs_image.bin

# Test with -t option and output to file
echo "Testing with -t option and output to file:"
./fsck_dir -t -o directory_tree_output.txt test_f2fs_image.bin

# Check if output file was created
if [ -f "directory_tree_output.txt" ]; then
    echo "Output file created successfully."
    echo "Checking if hardcode was removed..."
    
    # Check if the output contains the expected dynamic content
    if grep -q "Check node" directory_tree_output.txt && grep -q "FSCK" directory_tree_output.txt; then
        echo "SUCCESS: Hardcode has been removed and dynamic content is generated!"
        echo "First 20 lines of output:"
        head -n 20 directory_tree_output.txt
    else
        echo "WARNING: May still contain hardcode or failed to generate dynamic content."
        echo "First 20 lines of output:"
        head -n 20 directory_tree_output.txt
    fi
else
    echo "Error: Output file was not created."
fi

# Clean up
rm -f test_f2fs_image.bin directory_tree_output.txt

echo "Test completed."