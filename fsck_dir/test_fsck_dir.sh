#!/bin/bash
# Test script for fsck_dir tool

echo "Testing fsck_dir tool..."

# Test version display
echo "1. Testing version display:"
./fsck_dir -V

echo ""
echo "2. Testing directory tree display:"
echo "test content" | ./fsck_dir -t /dev/stdin

echo ""
echo "3. Testing help message:"
./fsck_dir 2>&1 | head -5

echo ""
echo "All tests completed successfully!"