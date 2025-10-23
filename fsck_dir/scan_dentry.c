#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>

// Basic types
typedef uint64_t	u64;
typedef uint32_t	u32;
typedef uint16_t	u16;
typedef uint8_t		u8;
typedef u32		nid_t;

// Little endian types
typedef u16 __le16;
typedef u32 __le32;
typedef u64 __le64;

#define F2FS_SUPER_OFFSET		1024
#define F2FS_BLKSIZE			4096
#define NR_DENTRY_IN_BLOCK		214
#define F2FS_SLOT_LEN			8
#define SIZE_OF_DENTRY_BITMAP	((NR_DENTRY_IN_BLOCK + 7) / 8)
#define SIZE_OF_RESERVED	(4096 - ((11 + F2FS_SLOT_LEN) * NR_DENTRY_IN_BLOCK + SIZE_OF_DENTRY_BITMAP))

// Byte order conversion functions
#define le16_to_cpu(x)  ((uint16_t)(x))
#define le32_to_cpu(x)  ((uint32_t)(x))

struct f2fs_dir_entry {
	__le32 hash_code;	/* hash code of file name */
	__le32 ino;		/* inode number */
	__le16 name_len;	/* length of file name */
	u8 file_type;	/* file type */
} __attribute__((packed));

struct f2fs_dentry_block {
	/* validity bitmap for directory entries in each block */
	u8 dentry_bitmap[SIZE_OF_DENTRY_BITMAP];
	u8 reserved[SIZE_OF_RESERVED];
	struct f2fs_dir_entry dentry[NR_DENTRY_IN_BLOCK];
	u8 filename[NR_DENTRY_IN_BLOCK][F2FS_SLOT_LEN];
} __attribute__((packed));

// File types
enum FILE_TYPE {
	F2FS_FT_UNKNOWN,
	F2FS_FT_REG_FILE,
	F2FS_FT_DIR,
	F2FS_FT_CHRDEV,
	F2FS_FT_BLKDEV,
	F2FS_FT_FIFO,
	F2FS_FT_SOCK,
	F2FS_FT_SYMLINK,
	F2FS_FT_MAX
};

// Helper function to test bit in little-endian bitmap
static inline int test_bit_le(int nr, const u8 *addr)
{
	return ((1 << (nr & 7)) & addr[nr >> 3]) != 0;
}

void print_filename(u8 (*filename_slots)[F2FS_SLOT_LEN], int slot_index, u32 name_len) {
    char name[256];
    int slots = (name_len + F2FS_SLOT_LEN - 1) / F2FS_SLOT_LEN;
    
    if (name_len >= sizeof(name))
        name_len = sizeof(name) - 1;
    
    // Copy name from slots
    for (int i = 0; i < slots && i * F2FS_SLOT_LEN < name_len; i++) {
        int copy_len = (i + 1) * F2FS_SLOT_LEN <= name_len ? F2FS_SLOT_LEN : name_len % F2FS_SLOT_LEN;
        if (copy_len == 0) copy_len = F2FS_SLOT_LEN;
        memcpy(name + i * F2FS_SLOT_LEN, filename_slots[slot_index + i], copy_len);
    }
    name[name_len] = '\0';
    
    printf("      Name: '%s' (len=%u)\n", name, name_len);
}

int scan_dentry_blocks(int fd, const char *filename) {
    struct stat st;
    if (fstat(fd, &st) < 0) {
        perror("fstat");
        return -1;
    }
    
    // Scan the image file block by block
    u8 *buffer = malloc(st.st_size);
    if (!buffer) {
        perror("malloc");
        return -1;
    }
    
    if (read(fd, buffer, st.st_size) != st.st_size) {
        perror("read");
        free(buffer);
        return -1;
    }
    
    printf("Scanning %s for directory entry blocks...\n", filename);
    printf("File size: %ld bytes (%ld blocks)\n", (long)st.st_size, (long)st.st_size / F2FS_BLKSIZE);
    
    // Scan each block
    for (off_t block_offset = 0; block_offset < st.st_size; block_offset += F2FS_BLKSIZE) {
        struct f2fs_dentry_block *dentry_block = (struct f2fs_dentry_block *)(buffer + block_offset);
        
        // Look for valid directory entries
        int valid_entries = 0;
        for (int i = 0; i < NR_DENTRY_IN_BLOCK; i++) {
            if (test_bit_le(i, dentry_block->dentry_bitmap)) {
                u32 ino = le32_to_cpu(dentry_block->dentry[i].ino);
                u16 name_len = le16_to_cpu(dentry_block->dentry[i].name_len);
                u8 file_type = dentry_block->dentry[i].file_type;
                
                // Basic validation
                if (ino != 0 && name_len > 0 && name_len <= 255 && file_type <= F2FS_FT_MAX) {
                    valid_entries++;
                }
            }
        }
        
        // If we found valid entries, print details
        if (valid_entries > 0) {
            printf("\nFound potential directory entry block at offset 0x%lx (%ld):\n", 
                   (long)block_offset, (long)block_offset / F2FS_BLKSIZE);
            printf("  Valid entries: %d\n", valid_entries);
            
            // Print details of each entry
            for (int i = 0; i < NR_DENTRY_IN_BLOCK; i++) {
                if (test_bit_le(i, dentry_block->dentry_bitmap)) {
                    u32 ino = le32_to_cpu(dentry_block->dentry[i].ino);
                    u16 name_len = le16_to_cpu(dentry_block->dentry[i].name_len);
                    u8 file_type = dentry_block->dentry[i].file_type;
                    u32 hash_code = le32_to_cpu(dentry_block->dentry[i].hash_code);
                    
                    // Basic validation
                    if (ino != 0 && name_len > 0 && name_len <= 255 && file_type <= F2FS_FT_MAX) {
                        const char *type_str = "UNKNOWN";
                        switch (file_type) {
                            case F2FS_FT_REG_FILE: type_str = "REG_FILE"; break;
                            case F2FS_FT_DIR: type_str = "DIR"; break;
                            case F2FS_FT_CHRDEV: type_str = "CHRDEV"; break;
                            case F2FS_FT_BLKDEV: type_str = "BLKDEV"; break;
                            case F2FS_FT_FIFO: type_str = "FIFO"; break;
                            case F2FS_FT_SOCK: type_str = "SOCK"; break;
                            case F2FS_FT_SYMLINK: type_str = "SYMLINK"; break;
                        }
                        
                        printf("  Entry %d:\n", i);
                        printf("    Inode: %u\n", ino);
                        printf("    Name length: %u\n", name_len);
                        printf("    File type: %s (%d)\n", type_str, file_type);
                        printf("    Hash code: 0x%x\n", hash_code);
                        print_filename((u8*)dentry_block->filename, i, name_len);
                    }
                }
            }
        }
    }
    
    free(buffer);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <f2fs_image>\n", argv[0]);
        return 1;
    }
    
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }
    
    int result = scan_dentry_blocks(fd, argv[1]);
    close(fd);
    return result;
}