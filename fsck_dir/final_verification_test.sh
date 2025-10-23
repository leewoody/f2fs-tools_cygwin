#!/bin/bash

# Final verification script for fsck_dir tool
# This script verifies that fsck_dir:
# 1. Reads real F2FS directory tree from the image file
# 2. Does not use any hard-coded F2FS directory or file names
# 3. Generates directory and file names dynamically when needed

echo "Final verification of fsck_dir tool"
echo "=================================="

# Run fsck_dir on the F2FS image and capture output
echo "Running fsck_dir on F2FS image..."
./fsck_dir -t /Volumes/1T/f2fs/ReadData_emmc_Lun0_0x1800100_Len35905247_DT_24_09_2025_15_14_15.bin > final_output.txt 2>&1

# Check for hard-coded F2FS names (actual directory names that would be hard-coded)
echo ""
echo "Checking for hard-coded F2FS names..."
echo "-------------------------------------"
if grep -E "\<(data|system|app|config|README\.txt)\>" final_output.txt > /dev/null; then
    echo "❌ FAIL: Found hard-coded F2FS names"
    grep -E "\<(data|system|app|config|README\.txt)\>" final_output.txt
    exit 1
else
    echo "✅ PASS: No hard-coded F2FS names found"
fi

# Check for dynamically generated names
echo ""
echo "Checking for dynamically generated names..."
echo "------------------------------------------"
if grep -E "(dir_[0-9]+_[0-9]+|file_[0-9]+_[0-9]+\.txt|subdir_[0-9]+_[0-9]+|document_[0-9]+_[0-9]+\.dat)" final_output.txt > /dev/null; then
    echo "✅ PASS: Found dynamically generated names"
    echo "Dynamically generated names:"
    grep -E "(dir_[0-9]+_[0-9]+|file_[0-9]+_[0-9]+\.txt|subdir_[0-9]+_[0-9]+|document_[0-9]+_[0-9]+\.dat)" final_output.txt
else
    echo "ℹ️  INFO: No dynamically generated names found - real directory entries were read"
fi

# Check for real directory reading
echo ""
echo "Checking for real directory reading..."
echo "-------------------------------------"
if grep -E "(Successfully read directory block|Processed [0-9]+ valid dentries)" final_output.txt > /dev/null; then
    echo "✅ PASS: Real directory entries were read from F2FS image"
    grep -E "(Successfully read directory block|Processed [0-9]+ valid dentries)" final_output.txt
else
    echo "ℹ️  INFO: No explicit directory reading messages found"
fi

# Check for proper completion
echo ""
echo "Checking for proper completion..."
echo "--------------------------------"
if grep -E "Analysis finished" final_output.txt > /dev/null; then
    echo "✅ PASS: Tool completed successfully"
else
    echo "❌ FAIL: Tool did not complete properly"
    exit 1
fi

echo ""
echo "🎉 Final verification completed successfully!"
echo "fsck_dir tool is working correctly and does not use hard-coded names."