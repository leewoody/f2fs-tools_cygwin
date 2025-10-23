#!/bin/bash

echo "Testing fsck_dir fix..."
echo "======================"

cd /Users/mac/Downloads/f2fs-tools_cygwin-main/fsck_dir

echo "1. Compiling fsck_dir..."
make clean && make
if [ $? -ne 0 ]; then
    echo "ERROR: Compilation failed"
    exit 1
fi

echo "2. Testing with userdata.img..."
echo "Output:"
./fsck_dir -t ../IMAGE/userdata.img

echo ""
echo "3. Testing with output redirection..."
./fsck_dir -t ../IMAGE/userdata.img -o test_output.txt
if [ -f test_output.txt ]; then
    echo "Output redirected to test_output.txt"
    echo "First 20 lines of output:"
    head -20 test_output.txt
else
    echo "ERROR: Output file not created"
fi

echo ""
echo "Test completed successfully!"