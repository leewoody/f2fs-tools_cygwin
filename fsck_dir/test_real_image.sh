#!/bin/bash

# Test script for fsck_dir tool with real F2FS image

echo "Testing fsck_dir tool with real F2FS image..."

# Check if fsck_dir executable exists
if [ ! -f "./fsck_dir" ]; then
    echo "Error: fsck_dir executable not found. Please compile first."
    exit 1
fi

# Create a simple test file to use as a mock F2FS image
echo "Creating a mock F2FS image for testing..."
dd if=/dev/zero of=test_f2fs_image.bin bs=1M count=50 2>/dev/null

# Add F2FS superblock magic number to the test image
printf '\x10\x20\xf5\xf2' | dd of=test_f2fs_image.bin bs=1 seek=1024 conv=notrunc 2>/dev/null

# Test with -t option to show directory tree
echo "Testing with -t option:"
./fsck_dir -t test_f2fs_image.bin

# Test with -t option and output to file
echo "Testing with -t option and output to file:"
./fsck_dir -t -o complete_fsck_dir_directory_treev4.txt test_f2fs_image.bin

# Check if output file was created
if [ -f "complete_fsck_dir_directory_treev4.txt" ]; then
    echo "Output file created successfully."
    echo "First 30 lines of output:"
    head -n 30 complete_fsck_dir_directory_treev4.txt
else
    echo "Error: Output file was not created."
fi

# Clean up
rm -f test_f2fs_image.bin

echo "Test completed."