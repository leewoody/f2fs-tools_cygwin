/**
 * simple_f2fs.h
 *
 * Very simplified F2FS definitions for fsck_dir tool
 */
#ifndef __SIMPLE_F2FS_H__
#define __SIMPLE_F2FS_H__

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
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

#ifndef bool
typedef u8		bool;
#endif

// Forward declarations
struct f2fs_device_info;

// Byte order conversion functions (simplified from f2fs_fs.h)
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define le16_to_cpu(x)  ((uint16_t)(x))
#define le32_to_cpu(x)  ((uint32_t)(x))
#define le64_to_cpu(x)  ((uint64_t)(x))
#define cpu_to_le16(x)  ((uint16_t)(x))
#define cpu_to_le32(x)  ((uint32_t)(x))
#define cpu_to_le64(x)  ((uint64_t)(x))
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
static inline uint16_t bswap_16(uint16_t val)
{
	return ((val & (uint16_t)0x00ffU) << 8)
		| ((val & (uint16_t)0xff00U) >> 8);
}

static inline uint32_t bswap_32(uint32_t val)
{
	return ((val & (uint32_t)0x000000ffUL) << 24)
		| ((val & (uint32_t)0x0000ff00UL) <<  8)
		| ((val & (uint32_t)0x00ff0000UL) >>  8)
		| ((val & (uint32_t)0xff000000UL) >> 24);
}

static inline uint64_t bswap_64(uint64_t val)
{
	return ((val & (uint64_t)0x00000000000000ffULL) << 56)
		| ((val & (uint64_t)0x000000000000ff00ULL) << 40)
		| ((val & (uint64_t)0x0000000000ff0000ULL) << 24)
		| ((val & (uint64_t)0x00000000ff000000ULL) <<  8)
		| ((val & (uint64_t)0x000000ff00000000ULL) >>  8)
		| ((val & (uint64_t)0x0000ff0000000000ULL) >> 24)
		| ((val & (uint64_t)0x00ff000000000000ULL) >> 40)
		| ((val & (uint64_t)0xff00000000000000ULL) >> 56);
}

#define le16_to_cpu(x)  bswap_16(x)
#define le32_to_cpu(x)  bswap_32(x)
#define le64_to_cpu(x)  bswap_64(x)
#define cpu_to_le16(x)  bswap_16(x)
#define cpu_to_le32(x)  bswap_32(x)
#define cpu_to_le64(x)  bswap_64(x)
#else
// Fallback for unknown byte order
#define le16_to_cpu(x)  ((uint16_t)(x))
#define le32_to_cpu(x)  ((uint32_t)(x))
#define le64_to_cpu(x)  ((uint64_t)(x))
#define cpu_to_le16(x)  ((uint16_t)(x))
#define cpu_to_le32(x)  ((uint32_t)(x))
#define cpu_to_le64(x)  ((uint64_t)(x))
#endif

// Constants
#define F2FS_NAME_LEN			255
#define F2FS_PRINT_NAMELEN		(F2FS_NAME_LEN * 3 + 1)
#define F2FS_ROOT_INO_NUM		3

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

// Node types
enum NODE_TYPE {
	TYPE_INODE = 37,
	TYPE_DIRECT_NODE = 43,
	TYPE_INDIRECT_NODE = 53,
	TYPE_DOUBLE_INDIRECT_NODE = 67,
	TYPE_XATTR = 77
};

// Simplified structures
struct f2fs_sb_info {
	u32 root_ino_num;
};

struct f2fs_inode {
	char dummy[4096];
};

struct extent_info {
	u32 fofs;
	u32 blk;
	u32 len;
};

struct child_info {
	u32 state;
	u32 links;
	u32 files;
	u32 pgofs;
	u8 dots;
	u8 dir_level;
	u32 p_ino;
	char p_name[F2FS_NAME_LEN + 1];
	u32 pp_ino;
	struct extent_info ei;
	u32 last_blk;
	u32 i_namelen;
};

struct f2fs_compr_blk_cnt {
	u32 cnt;
	u32 cheader_pgofs;
};

#define CHEADER_PGOFS_NONE	0xFFFFFFFF

// Min macro
#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

// False definition
#ifndef false
#define false 0
#endif

// Function declarations
extern int f2fs_init_sbi(struct f2fs_sb_info **sbi, const char *dev, 
                        struct f2fs_device_info *devs, int ndevs);
extern void f2fs_free_sbi(struct f2fs_sb_info *sbi);
extern void fsck_init(struct f2fs_sb_info *sbi);
extern void fsck_free(struct f2fs_sb_info *sbi);
extern int fsck_chk_node_blk(struct f2fs_sb_info *sbi, struct f2fs_inode *inode,
		u32 nid, enum FILE_TYPE ftype, enum NODE_TYPE type,
		u32 *blk_cnt, struct f2fs_compr_blk_cnt *cbc,
		struct child_info *child);
extern void pretty_print_filename(const u8 *raw_name, u32 len,
			   char out[F2FS_PRINT_NAMELEN], int enc_name);
extern void show_version(const char *prog);

#endif /* __SIMPLE_F2FS_H__ */