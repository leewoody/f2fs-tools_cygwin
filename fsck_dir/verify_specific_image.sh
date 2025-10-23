#!/bin/bash

# Script to verify a specific F2FS image file

IMAGE_PATH="/Volumes/1T/f2fs/ReadData_emmc_Lun0_0x1800100_Len35905247_DT_24_09_2025_15_14_15.bin"
OUTPUT_FILE="specific_image_directory_tree.txt"

echo "Verifying F2FS image: $IMAGE_PATH"

# Check if the image file exists
if [ ! -f "$IMAGE_PATH" ]; then
    echo "Error: F2FS image file not found at $IMAGE_PATH"
    exit 1
fi

# Check if fsck_dir executable exists
if [ ! -f "./fsck_dir" ]; then
    echo "Error: fsck_dir executable not found. Please compile first."
    exit 1
fi

echo "Image file size: $(du -h "$IMAGE_PATH" | cut -f1)"

# Run fsck_dir with the -t option to show directory tree
echo "Running fsck_dir to analyze the F2FS image..."
echo "This may take some time depending on the image size..."

# Execute fsck_dir and save output to file
./fsck_dir -t -o "$OUTPUT_FILE" "$IMAGE_PATH"

# Check if output file was created
if [ -f "$OUTPUT_FILE" ]; then
    echo "Analysis completed successfully."
    echo "Directory tree saved to $OUTPUT_FILE"
    
    # Show statistics
    echo "Output file size: $(du -h "$OUTPUT_FILE" | cut -f1)"
    echo "Total lines in output: $(wc -l < "$OUTPUT_FILE")"
    
    # Show first 50 lines of the output
    echo "First 50 lines of directory tree:"
    head -n 50 "$OUTPUT_FILE"
    
    # Look for specific files that we expect to find
    echo "Searching for specific files:"
    grep -E "(Gerrit|face\.jpeg|subdir|test_file|verification)" "$OUTPUT_FILE" || echo "No specific files found in output"
else
    echo "Error: Analysis failed or output file was not created."
fi

echo "Verification process completed."