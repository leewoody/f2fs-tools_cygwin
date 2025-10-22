# Android System Directories in test_copy.bin

## Complete Directory Listing

**Total Android System Directories: 46**

### Core System Directories

| Directory | Inode | Description |
|-----------|-------|-------------|
| `unencrypted/` | 0x7 | Unencrypted system data and keys |
| `bootchart/` | 0x11 | Boot performance monitoring data |
| `apex/` | 0x12 | Android Package Extension modules |
| `app-staging/` | 0x16 | Application staging area |
| `misc/` | 0x17 | Miscellaneous encrypted system data |
| `local/` | 0x34 | Local system data |
| `preloads/` | 0x43 | Pre-loaded applications |
| `vendor/` | 0x44 | Vendor-specific data |
| `vendor_ce/` | 0x45 | Vendor credential encrypted |
| `vendor_de/` | 0x46 | Vendor device encrypted |

### Application & Data Directories

| Directory | Inode | Description |
|-----------|-------|-------------|
| `data/` | 0x4a | **Primary user and application data (258 subdirs)** |
| `app-private/` | 0x4b | Private application data |
| `app-ephemeral/` | 0x4c | Ephemeral app data |
| `app-asec/` | 0x4d | Android Secure Container |
| `app-lib/` | 0x4e | Application libraries |
| `app/` | 0x4f | Installed applications |

### System Services & Configuration

| Directory | Inode | Description |
|-----------|-------|-------------|
| `property/` | 0x50 | System properties |
| `system/` | 0x60 | System application data |
| `system_de/` | 0x64 | System device encrypted |
| `system_ce/` | 0x65 | System credential encrypted |
| `misc_de/` | 0x66 | Misc device encrypted |
| `misc_ce/` | 0x67 | Misc credential encrypted |
| `server_configurable_flags/` | 0xda | System configuration flags |
| `dpm/` | 0xca | Device Policy Manager |

### Cache & Storage

| Directory | Inode | Description |
|-----------|-------|-------------|
| `cache/` | 0x6d | System cache |
| `dalvik-cache/` | 0x54 | Dalvik/ART runtime cache |
| `resource-cache/` | 0x57 | Resource cache |

### Media & User Data

| Directory | Inode | Description |
|-----------|-------|-------------|
| `media/` | 0x6b | Media storage |
| `user/` | 0x68 | User profiles |
| `user_de/` | 0x69 | User device encrypted |

### System Updates & Recovery

| Directory | Inode | Description |
|-----------|-------|-------------|
| `ota/` | 0x55 | Over-The-Air update data |
| `ota_package/` | 0x56 | OTA package storage |
| `rollback/` | 0x516 | System rollback data |
| `rollback-observer/` | 0x515 | Rollback monitoring |
| `gsi/` | 0xc0 | Generic System Image |

### Debugging & Development

| Directory | Inode | Description |
|-----------|-------|-------------|
| `tombstones/` | 0x51 | Crash dump files |
| `anr/` | 0x5b | Application Not Responding logs |
| `adb/` | 0x8d | Android Debug Bridge data |

### Security & DRM

| Directory | Inode | Description |
|-----------|-------|-------------|
| `drm/` | 0x59 | Digital Rights Management |
| `mediadrm/` | 0x5a | Media DRM data |
| `backup/` | 0x5e | System backup data |
| `ss/` | 0x5f | Screenshot/security data |

### Communication & Connectivity

| Directory | Inode | Description |
|-----------|-------|-------------|
| `nfc/` | 0x5c | Near Field Communication |

### Temporary & Runtime

| Directory | Inode | Description |
|-----------|-------|-------------|
| `per_boot/` | 0x5a7 | Per-boot temporary data |
| `lost+found/` | 0x58 | Filesystem recovery |

### System Metadata

| Directory | Inode | Description |
|-----------|-------|-------------|
| `.layout_version` | 0xf4 | Filesystem layout version |

## Directory Structure Analysis

### Encryption Levels
- **Level 0 (Unencrypted)**: All 46 root-level system directories
- **Level 4 (Encrypted)**: Thousands of subdirectories under `/data/` and `/misc/`

### Key Observations

1. **Standard Android Layout**: This is a typical Android `/data` partition structure
2. **Modern Android Features**: Includes APEX modules, GSI support, rollback mechanisms
3. **Security Architecture**: Full File-Based Encryption (FBE) implementation
4. **Vendor Integration**: Separate vendor partitions with encryption support
5. **Development Support**: Complete debugging and development infrastructure

### Most Important Directories

1. **`data/`** - Contains 258 encrypted subdirectories with all user and app data
2. **`misc/`** - Extensive encrypted system configuration and state data
3. **`system/`** - Core system application data
4. **`apex/`** - Modern Android modular system components
5. **`cache/`** - Runtime cache and temporary data

### Total File Count Estimate
- **Root Level**: 46 system directories + 6 added test files = 52 entries
- **Subdirectories**: 2000+ encrypted entries (mainly under `/data/` and `/misc/`)
- **Total Filesystem Nodes**: 2,121

This represents a complete, production Android data partition with all standard system services, security frameworks, and user data structures intact.