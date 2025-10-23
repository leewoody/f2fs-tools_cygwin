#!/bin/bash

# Comprehensive test script for fsck_dir tool functionality

echo "========================================"
echo "Testing fsck_dir tool functionality"
echo "========================================"

# Check if fsck_dir executable exists
if [ ! -f "./fsck_dir" ]; then
    echo "Error: fsck_dir executable not found. Please compile first."
    exit 1
fi

# Test 1: Basic functionality with -t option
echo "Test 1: Basic functionality with -t option"
echo "----------------------------------------"
./fsck_dir -t /Volumes/1T/f2fs/test_copy.bin > test_output1.txt 2>&1
if [ $? -eq 0 ]; then
    echo "PASS: Basic functionality test"
else
    echo "FAIL: Basic functionality test"
fi

# Test 2: Output redirection with -o option
echo ""
echo "Test 2: Output redirection with -o option"
echo "----------------------------------------"
./fsck_dir -t -o test_output2.txt /Volumes/1T/f2fs/test_copy.bin
if [ -f "test_output2.txt" ] && [ -s "test_output2.txt" ]; then
    echo "PASS: Output redirection test"
    echo "First 10 lines of output:"
    head -n 10 test_output2.txt
else
    echo "FAIL: Output redirection test"
fi

# Test 3: Version information
echo ""
echo "Test 3: Version information"
echo "----------------------------------------"
./fsck_dir -V > test_version.txt 2>&1
if grep -q "F2FS Tools" test_version.txt; then
    echo "PASS: Version information test"
    cat test_version.txt
else
    echo "FAIL: Version information test"
fi

# Test 4: Help/usage information
echo ""
echo "Test 4: Help/usage information"
echo "----------------------------------------"
./fsck_dir 2> test_usage.txt
if grep -q "Usage" test_usage.txt; then
    echo "PASS: Help/usage information test"
    grep "Usage" test_usage.txt
else
    echo "FAIL: Help/usage information test"
fi

# Test 5: Error handling for missing device
echo ""
echo "Test 5: Error handling for missing device"
echo "----------------------------------------"
./fsck_dir -t 2> test_error.txt
if grep -q "Device not specified" test_error.txt; then
    echo "PASS: Error handling test"
else
    echo "FAIL: Error handling test"
fi

# Clean up test files
echo ""
echo "Cleaning up test files..."
rm -f test_output1.txt test_output2.txt test_version.txt test_usage.txt test_error.txt

echo ""
echo "========================================"
echo "All tests completed"
echo "========================================"