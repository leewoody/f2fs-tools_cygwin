# fsck_dir - Simplified F2FS Directory Tree Viewer

## Overview

fsck_dir is a simplified version of the F2FS filesystem checker that focuses only on displaying the directory tree structure of an F2FS image. It's a lightweight alternative to the full fsck.f2fs tool, containing only the essential functionality for viewing directory structures.

This version is completely self-contained and can be built independently of the main F2FS tools project.

## Features

- Displays F2FS directory tree structure
- Completely standalone with no external dependencies on the main F2FS tools
- Easy to build and use
- Compatible with existing F2FS images

## Directory Structure

```
fsck_dir/
├── fsck_dir.c              # Main source code
├── libf2fs_simplified.c    # Simplified F2FS library functions
├── Makefile                # Build configuration
├── README.md               # This file
└── include/                # Header files
    ├── simple_f2fs.h       # Simplified F2FS filesystem definitions
    └── simple_fsck.h       # Simplified F2FS checker definitions
```

## Building

To build fsck_dir, you need a standard C compiler:

```bash
# On Ubuntu/Debian:
sudo apt-get install build-essential

# On CentOS/RHEL:
sudo yum install gcc make

# On macOS with Xcode:
# Xcode command line tools are sufficient
```

Then compile the tool:

```bash
cd /path/to/f2fs-tools/fsck_dir
make
```

## Usage

```bash
# Display directory tree of an F2FS image
./fsck_dir -t /path/to/image.f2fs

# Show version
./fsck_dir -V
```

## Example Output

```
/
|-- lost+found <ino = 0x58>, <encrypted (0)>
|-- unencrypted <ino = 0x7>, <encrypted (0)>
|   |-- key <ino = 0x8>, <encrypted (0)>
|   |   |-- version <ino = 0x9>, <encrypted (0)>
|   |   `-- secdiscardable <ino = 0xa>, <encrypted (0)>
|   |-- mode <ino = 0xe>, <encrypted (0)>
|   `-- per_boot_ref <ino = 0x10>, <encrypted (0)>
|-- apex <ino = 0x12>, <encrypted (0)>
|   |-- active <ino = 0x13>, <encrypted (0)>
|   |-- backup <ino = 0x14>, <encrypted (0)>
|   `-- sessions <ino = 0x15>, <encrypted (0)>
|-- data <ino = 0x4a>, <encrypted (0)>
|   `-- app <ino = 0x4f>, <encrypted (0)>
`-- system <ino = 0x60>, <encrypted (0)>
    `-- framework <ino = 0x61>, <encrypted (0)>
```

## Implementation Details

This tool is a completely standalone version that contains only the essential functionality for browsing F2FS directory structures. It includes a simplified version of the F2FS library with only the functions needed for directory tree display.

The tool removes all repair, validation, and other complex features to provide a lightweight solution for browsing F2FS directory structures.

## Requirements

- Standard C compiler (GCC/Clang)
- No dependency on the main F2FS tools project
- No external libraries required

## License

This tool is distributed under the same license as the original F2FS tools.