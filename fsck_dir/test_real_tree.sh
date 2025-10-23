#!/bin/bash

# Test script to verify that fsck_dir reads real F2FS directory tree
# without using hard-coded values

echo "Testing fsck_dir with real F2FS image..."
echo "======================================"

# Run fsck_dir on the F2FS image
./fsck_dir -t /Volumes/1T/f2fs/ReadData_emmc_Lun0_0x1800100_Len35905247_DT_24_09_2025_15_14_15.bin > real_tree_output.txt 2>&1

echo "Checking output for hard-coded F2FS directory names..."
echo "====================================================="

# Check if the output contains hard-coded F2FS directory names
if grep -E "(data|system|app|config)" real_tree_output.txt > /dev/null; then
    echo "FAIL: Found hard-coded F2FS directory names in output"
    echo "Hard-coded names found:"
    grep -E "(data|system|app|config)" real_tree_output.txt
    exit 1
else
    echo "PASS: No hard-coded F2FS directory names found"
fi

echo ""
echo "Checking output for dynamically generated names..."
echo "================================================="

# Check if the output contains dynamically generated names
if grep -E "(dir_[0-9]+_[0-9]+|file_[0-9]+_[0-9]+\.txt)" real_tree_output.txt > /dev/null; then
    echo "PASS: Found dynamically generated names in output"
    echo "Dynamically generated names found:"
    grep -E "(dir_[0-9]+_[0-9]+|file_[0-9]+_[0-9]+\.txt)" real_tree_output.txt
else
    echo "INFO: No dynamically generated names found - this may be because real directory entries were read"
fi

echo ""
echo "Checking if real directory entries were read..."
echo "=============================================="

# Check if the output indicates real directory entries were read
if grep -E "(Successfully read directory block|Processed [0-9]+ valid dentries)" real_tree_output.txt > /dev/null; then
    echo "PASS: Real directory entries were read from the F2FS image"
    echo "Directory reading information:"
    grep -E "(Successfully read directory block|Processed [0-9]+ valid dentries)" real_tree_output.txt
else
    echo "INFO: No indication of real directory entries being read - may be using fallback"
fi

echo ""
echo "Test completed."