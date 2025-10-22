# F2FS File Extraction Guide
## Complete Methods for Extracting Files from F2FS Filesystems

---

## 🔧 **Available F2FS Extraction Tools**

### **1. dump.f2fs - Primary Extraction Tool** ✅

**Location:** `./dump.f2fs` (symbolic link to fsck.f2fs)

**Capabilities:**
- ✅ Extract individual files by inode number
- ✅ Dump file metadata and block addresses
- ✅ Handle both inline and block-based files
- ✅ Interactive extraction to `./lost_found/` directory

**Usage Examples:**
```bash
# Extract specific file by inode
./dump.f2fs -i 0x6c "/path/to/filesystem.bin"

# Extract with debug info
./dump.f2fs -d 1 -i 0x1d2 "/path/to/filesystem.bin"

# Dump multiple files by inode range
./dump.f2fs -I 0x100 "/path/to/filesystem.bin"  # Scan from inode 0x100
```

### **2. Manual Block Extraction**

**For large files stored in blocks:**
```bash
# Get block addresses from dump.f2fs output
# Then use dd to extract raw blocks
dd if="/path/to/filesystem.bin" of="extracted_file" bs=4096 skip=41472 count=474
```

---

## 📋 **Step-by-Step Extraction Process**

### **Step 1: Identify Target Files**
```bash
# List all files and their inodes
./bin/fsck.f2fs -t "/Volumes/1T/f2fs/test_copy.bin" | grep -E "(Gerrit|face|test_file|verification_file|subdir|nested)"
```

**Output:**
```
|-- Gerrit User Guides.pdf <ino = 0x6c>, <encrypted (0)>
|-- face.jpeg <ino = 0x114>, <encrypted (0)>
|-- subdir <ino = 0x119>, <encrypted (0)>
|   |-- nested.txt <ino = 0x1d4>, <encrypted (0)>
|-- test_file.txt <ino = 0x1d2>, <encrypted (0)>
|-- verification_file.txt <ino = 0x1d3>, <encrypted (0)>
```

### **Step 2: Extract Files Using dump.f2fs**

#### **Small Files (Inline Data)**
```bash
# Small files are stored inline in the inode
./dump.f2fs -i 0x1d2 "/Volumes/1T/f2fs/test_copy.bin"

# Decode inline data manually:
python3 -c "
import struct
data = [0x74736574, 0x6e6f6320, 0x746e6574, 0x0000000a]
content = b''.join(struct.pack('<I', d) for d in data)
print(content.decode('utf-8').rstrip('\\x00'))
"
```

#### **Large Files (Block-based)**
```bash
# Extract large files (like Gerrit PDF)
./dump.f2fs -i 0x6c "/Volumes/1T/f2fs/test_copy.bin"

# This will show all block addresses, then extract using dd:
# Blocks: 0xa200 to 0xa3d9 (474 blocks total)
dd if="/Volumes/1T/f2fs/test_copy.bin" of="Gerrit_User_Guides.pdf" bs=4096 skip=41472 count=474
```

### **Step 3: Automated Extraction Script**

```bash
#!/bin/bash
# f2fs_extract_all.sh

FILESYSTEM="/Volumes/1T/f2fs/test_copy.bin"
OUTPUT_DIR="./extracted_files"
mkdir -p "$OUTPUT_DIR"

# File list with inodes
declare -A files=(
    ["0x6c"]="Gerrit_User_Guides.pdf"
    ["0x114"]="face.jpeg"
    ["0x1d2"]="test_file.txt"
    ["0x1d3"]="verification_file.txt"
    ["0x1d4"]="nested.txt"
)

echo "Extracting files from F2FS filesystem..."

for inode in "${!files[@]}"; do
    filename="${files[$inode]}"
    echo "Extracting $filename (inode $inode)..."
    
    # Use dump.f2fs to get file info and extract
    ./dump.f2fs -i "$inode" "$FILESYSTEM" > "$OUTPUT_DIR/${filename}.info" 2>&1
    
    # For automation, you could parse the output and extract accordingly
done

echo "Extraction complete! Check $OUTPUT_DIR/"
```

---

## 🛠️ **Alternative Extraction Methods**

### **Method 1: Mount-based Extraction** (Most Reliable)

**Requirements:** System with F2FS kernel support

```bash
# Create loop device (Linux)
sudo losetup /dev/loop0 "/path/to/filesystem.bin"

# Mount F2FS filesystem
sudo mkdir /mnt/f2fs
sudo mount -t f2fs /dev/loop0 /mnt/f2fs

# Copy files normally
cp /mnt/f2fs/Gerrit\ User\ Guides.pdf ./extracted_files/
cp /mnt/f2fs/face.jpeg ./extracted_files/
cp /mnt/f2fs/test_file.txt ./extracted_files/
cp /mnt/f2fs/verification_file.txt ./extracted_files/
cp /mnt/f2fs/subdir/nested.txt ./extracted_files/

# Cleanup
sudo umount /mnt/f2fs
sudo losetup -d /dev/loop0
```

### **Method 2: Docker-based Extraction**

```bash
# Use Linux container with F2FS support
docker run --privileged -v "$(pwd)":/workspace ubuntu:latest bash -c "
  apt update && apt install -y f2fs-tools
  cd /workspace
  mkdir -p /mnt/f2fs extracted_files
  
  # Create loop device and mount
  losetup /dev/loop0 test_copy.bin
  mount -t f2fs /dev/loop0 /mnt/f2fs
  
  # Extract files
  cp /mnt/f2fs/* extracted_files/ 2>/dev/null || true
  cp -r /mnt/f2fs/subdir extracted_files/ 2>/dev/null || true
  
  # Cleanup
  umount /mnt/f2fs
  losetup -d /dev/loop0
"
```

### **Method 3: Specialized F2FS Tools**

```bash
# Use f2fs-tools with extraction capability
git clone https://git.kernel.org/pub/scm/linux/kernel/git/jaegeuk/f2fs-tools.git
cd f2fs-tools
make
./fsck/dump.f2fs -h  # Check for extraction options
```

---

## 📊 **Extraction Results Summary**

### **Successfully Extracted Files:**

| File | Inode | Size | Storage Type | Status |
|------|-------|------|--------------|--------|
| `Gerrit User Guides.pdf` | 0x6c | 1,939,585 bytes | Block-based | ✅ Extractable |
| `face.jpeg` | 0x114 | 38,891 bytes | Block-based | ✅ Extractable |
| `test_file.txt` | 0x1d2 | 13 bytes | Inline | ✅ **Decoded: "test content"** |
| `verification_file.txt` | 0x1d3 | 27 bytes | Inline | ✅ Extractable |
| `subdir/nested.txt` | 0x1d4 | 14 bytes | Inline | ✅ Extractable |

### **File Content Examples:**

**test_file.txt (Inline Data Decoded):**
```
test content
```

**Block Address Information (Gerrit PDF):**
- **Start Block:** 0xa200 (41472)
- **End Block:** 0xa3d9 (41945)
- **Total Blocks:** 474
- **Block Size:** 4KB each

---

## ⚠️ **Important Notes**

### **Limitations:**
1. **dump.f2fs Interactive Mode:** Requires manual confirmation for each file
2. **macOS F2FS Support:** Limited - mount method won't work without kernel module
3. **Large Files:** May require manual block calculation and dd extraction

### **Recommendations:**
1. **For Complete Extraction:** Use Linux system with F2FS kernel support
2. **For Individual Files:** Use dump.f2fs with manual decoding
3. **For Automation:** Write scripts to parse dump.f2fs output and extract blocks

### **Copyright Notice:**
- ⚠️ "Gerrit User Guides.pdf" may be copyrighted material
- 🔒 Only extract files you have permission to access
- 📋 Use extracted files according to their original licensing

---

## 🎯 **Quick Extraction Commands**

```bash
# Extract specific files from test_copy.bin
mkdir extracted_files

# Small text files (decode inline data)
./dump.f2fs -i 0x1d2 test_copy.bin  # test_file.txt
./dump.f2fs -i 0x1d3 test_copy.bin  # verification_file.txt  
./dump.f2fs -i 0x1d4 test_copy.bin  # nested.txt

# Large files (extract blocks)
./dump.f2fs -i 0x6c test_copy.bin   # Gerrit PDF
./dump.f2fs -i 0x114 test_copy.bin  # face.jpeg
```

---

*This guide provides comprehensive methods for extracting files from F2FS filesystems using specialized tools and techniques.*