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
#define NAT_ENTRY_PER_BLOCK		(F2FS_BLKSIZE / 9)  // 9 bytes per entry

// Byte order conversion functions
#define le32_to_cpu(x)  ((uint32_t)(x))

struct f2fs_super_block {
	__le32 magic;			/* Magic Number */
	__le16 major_ver;		/* Major Version */
	__le16 minor_ver;		/* Minor Version */
	__le32 log_sectorsize;		/* log2 sector size in bytes */
	__le32 log_sectors_per_block;	/* log2 # of sectors per block */
	__le32 log_blocksize;		/* log2 block size in bytes */
	__le32 log_blocks_per_seg;	/* log2 # of blocks per segment */
	__le32 segs_per_sec;		/* # of segments per section */
	__le32 secs_per_zone;		/* # of sections per zone */
	__le32 checksum_offset;		/* checksum offset inside super block */
	__le64 block_count;		/* total # of user blocks */
	__le32 section_count;		/* total # of sections */
	__le32 segment_count;		/* total # of segments */
	__le32 segment_count_ckpt;	/* # of segments for checkpoint */
	__le32 segment_count_sit;	/* # of segments for SIT */
	__le32 segment_count_nat;	/* # of segments for NAT */
	__le32 segment_count_ssa;	/* # of segments for SSA */
	__le32 segment_count_main;	/* # of segments for main area */
	__le32 segment0_blkaddr;	/* start block address of segment 0 */
	__le32 cp_blkaddr;		/* start block address of checkpoint */
	__le32 sit_blkaddr;		/* start block address of SIT */
	__le32 nat_blkaddr;		/* start block address of NAT */
	__le32 ssa_blkaddr;		/* start block address of SSA */
	__le32 main_blkaddr;		/* start block address of main area */
	__le32 root_ino;		/* root inode number */
	__le32 node_ino;		/* node inode number */
	__le32 meta_ino;		/* meta inode number */
	u8 uuid[16];			/* 128-bit uuid for volume */
	__le16 volume_name[512];	/* volume name */
	__le32 extension_count;		/* # of extensions below */
	u8 extension_list[64][8];	/* extension array */
	__le32 cp_payload;
	u8 version[16];		/* the kernel version */
	u8 init_version[16];		/* the initial kernel version */
	__le32 feature;			/* defined features */
	u8 encryption_level;		/* versioning level for encryption */
	u8 encrypt_pw_salt[16];	/* Salt used for string2key algorithm */
	struct {
		u8 path[64];
		__le32 total_segments;
	} devs[8];			/* device list */
	__le32 qf_ino[3];		/* quota inode numbers */
	u8 hot_ext_count;		/* # of hot file extension */
	__le16  s_encoding;		/* Filename charset encoding */
	__le16  s_encoding_flags;	/* Filename charset encoding flags */
	u8 reserved[306];		/* valid reserved region */
	__le32 crc;			/* checksum of superblock */
};

struct f2fs_nat_entry {
	u8 version;		/* latest version of cached nat entry */
	__le32 ino;		/* inode number */
	__le32 block_addr;	/* block address */
} __attribute__((packed));

struct f2fs_nat_block {
	struct f2fs_nat_entry entries[NAT_ENTRY_PER_BLOCK];
};

#define NAT_BLOCK_OFFSET(start_nid) ((start_nid) / NAT_ENTRY_PER_BLOCK)

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
    
    // Read superblock
    struct f2fs_super_block sb;
    if (pread(fd, &sb, sizeof(sb), F2FS_SUPER_OFFSET) != sizeof(sb)) {
        perror("pread superblock");
        close(fd);
        return 1;
    }
    
    // Check magic number
    if (le32_to_cpu(sb.magic) != 0xF2F52010) {
        printf("Invalid F2FS magic number: 0x%x\n", le32_to_cpu(sb.magic));
        close(fd);
        return 1;
    }
    
    printf("F2FS Superblock Info:\n");
    printf("  Magic: 0x%x\n", le32_to_cpu(sb.magic));
    printf("  log_blocks_per_seg: %u\n", le32_to_cpu(sb.log_blocks_per_seg));
    printf("  nat_blkaddr: %u\n", le32_to_cpu(sb.nat_blkaddr));
    printf("  sit_blkaddr: %u\n", le32_to_cpu(sb.sit_blkaddr));
    printf("  ssa_blkaddr: %u\n", le32_to_cpu(sb.ssa_blkaddr));
    printf("  main_blkaddr: %u\n", le32_to_cpu(sb.main_blkaddr));
    printf("  root_ino: %u\n", le32_to_cpu(sb.root_ino));
    printf("  node_ino: %u\n", le32_to_cpu(sb.node_ino));
    printf("  meta_ino: %u\n", le32_to_cpu(sb.meta_ino));
    printf("  segment_count_nat: %u\n", le32_to_cpu(sb.segment_count_nat));
    
    // Check multiple NAT blocks
    u32 nat_blkaddr = le32_to_cpu(sb.nat_blkaddr);
    u32 log_blocks_per_seg = le32_to_cpu(sb.log_blocks_per_seg);
    u32 segment_count_nat = le32_to_cpu(sb.segment_count_nat);
    
    printf("\nChecking NAT blocks...\n");
    for (u32 seg = 0; seg < segment_count_nat && seg < 5; seg++) {
        // Calculate block address for this segment
        u32 block_addr = nat_blkaddr + (seg << log_blocks_per_seg);
        printf("  NAT Segment %u: block_addr = %u\n", seg, block_addr);
        
        // Read NAT block
        struct f2fs_nat_block nat_block;
        off_t offset = (off_t)block_addr * F2FS_BLKSIZE;
        
        if (pread(fd, &nat_block, sizeof(nat_block), offset) != sizeof(nat_block)) {
            printf("    Failed to read NAT block\n");
            continue;
        }
        
        // Check for non-zero entries
        int non_zero_count = 0;
        for (int i = 0; i < NAT_ENTRY_PER_BLOCK && i < 20; i++) {
            struct f2fs_nat_entry *e = &nat_block.entries[i];
            if (le32_to_cpu(e->ino) != 0 || le32_to_cpu(e->block_addr) != 0) {
                printf("    [%d] ino=0x%x, block_addr=0x%x\n", 
                       i, le32_to_cpu(e->ino), le32_to_cpu(e->block_addr));
                non_zero_count++;
            }
        }
        
        if (non_zero_count > 0) {
            printf("    Found %d non-zero entries in this block\n", non_zero_count);
        } else {
            printf("    All entries are zero\n");
        }
    }
    
    close(fd);
    return 0;
}