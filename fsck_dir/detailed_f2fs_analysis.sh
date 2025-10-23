#!/bin/bash

# Detailed F2FS analysis script

echo "=== Detailed F2FS Analysis ==="

# Check if fsck_dir executable exists
if [ ! -f "./fsck_dir" ]; then
    echo "Error: fsck_dir executable not found. Please compile first."
    exit 1
fi

# Check if the F2FS image file exists
if [ ! -f "/Volumes/1T/f2fs/test_copy.bin" ]; then
    echo "Error: F2FS image file not found."
    exit 1
fi

echo "F2FS image file found:"
ls -lh /Volumes/1T/f2fs/test_copy.bin

echo ""
echo "Running detailed F2FS analysis..."
echo "================================"

# Run fsck_dir and capture both stdout and stderr
./fsck_dir -t /Volumes/1T/f2fs/test_copy.bin 2>&1 | tee detailed_analysis_output.txt

echo ""
echo "Analysis complete. Checking output..."

# Check if we have directory tree content
if grep -q "Successfully read real directory entries" detailed_analysis_output.txt; then
    echo "✅ Tool successfully read directory entries from F2FS image"
    
    # Count lines in output
    line_count=$(wc -l < detailed_analysis_output.txt)
    echo "Output contains $line_count lines"
    
    # Show first 30 lines
    echo ""
    echo "First 30 lines of output:"
    head -n 30 detailed_analysis_output.txt
    
    # Show last 20 lines
    echo ""
    echo "Last 20 lines of output:"
    tail -n 20 detailed_analysis_output.txt
    
    # Check for directory entries
    if grep -q "--" detailed_analysis_output.txt; then
        echo ""
        echo "✅ Directory tree entries found:"
        grep -- "--" detailed_analysis_output.txt | head -n 10
    else
        echo ""
        echo "ℹ️  No directory tree entries found in output"
    fi
else
    echo "❌ Tool did not successfully read directory entries"
    cat detailed_analysis_output.txt
fi

# Also run with output to file
echo ""
echo "Running with output to file..."
./fsck_dir -t -o complete_fsck_dir_directory_treev2.txt /Volumes/1T/f2fs/test_copy.bin

echo ""
echo "File output analysis:"
if [ -f "complete_fsck_dir_directory_treev2.txt" ]; then
    echo "✅ Output file created"
    line_count=$(wc -l < complete_fsck_dir_directory_treev2.txt)
    echo "File contains $line_count lines"
    
    if [ $line_count -gt 5 ]; then
        echo "First 20 lines of file:"
        head -n 20 complete_fsck_dir_directory_treev2.txt
    else
        echo "Full content of file:"
        cat complete_fsck_dir_directory_treev2.txt
    fi
else
    echo "❌ Output file not created"
fi

echo ""
echo "=== Analysis Complete ==="