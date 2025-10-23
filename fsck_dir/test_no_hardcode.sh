#!/bin/bash

# Test script to verify that fsck_dir doesn't use hardcoded F2FS directory names
echo "Testing fsck_dir for hardcoded F2FS directory names..."

# Run fsck_dir on the test image and capture output
./fsck_dir -t /Volumes/1T/f2fs/test_copy.bin > test_output.txt 2>&1

# Check if output contains any hardcoded F2FS directory names
echo "Checking for hardcoded F2FS directory names..."

# Common F2FS directory names that should not appear in output
HARDCODED_NAMES=("data" "system" "app" "config" "README.txt")

FOUND_HARDCODED=false
for name in "${HARDCODED_NAMES[@]}"; do
    if grep -q "$name" test_output.txt; then
        echo "ERROR: Found hardcoded name '$name' in output"
        FOUND_HARDCODED=true
    fi
done

if [ "$FOUND_HARDCODED" = false ]; then
    echo "SUCCESS: No hardcoded F2FS directory names found in output"
else
    echo "FAILURE: Hardcoded F2FS directory names found in output"
    echo "Output content:"
    cat test_output.txt
fi

# Clean up
rm -f test_output.txt
