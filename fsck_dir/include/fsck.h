/**
 * fsck.h
 *
 * Copyright (c) 2013 Samsung Electronics Co., Ltd.
 *             http://www.samsung.com/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef _FSCK_H_
#define _FSCK_H_

#include "f2fs_fs.h"

enum {
	FSCK_SUCCESS                 = 0,
	FSCK_ERROR_CORRECTED         = 1 << 0,
	FSCK_SYSTEM_SHOULD_REBOOT    = 1 << 1,
	FSCK_ERRORS_LEFT_UNCORRECTED = 1 << 2,
	FSCK_OPERATIONAL_ERROR       = 1 << 3,
	FSCK_USAGE_OR_SYNTAX_ERROR   = 1 << 4,
	FSCK_USER_CANCELLED          = 1 << 5,
	FSCK_SHARED_LIB_ERROR        = 1 << 7,
};

#define FSCK_UNMATCHED_EXTENT		0x00000001
#define FSCK_INLINE_INODE		0x00000002

enum {
	PREEN_MODE_0,
	PREEN_MODE_1,
	PREEN_MODE_2,
	PREEN_MODE_MAX
};

enum {
	NOERROR,
	EWRONG_OPT,
	ENEED_ARG,
	EUNKNOWN_OPT,
	EUNKNOWN_ARG,
};

enum SB_ADDR {
	SB0_ADDR = 0,
	SB1_ADDR,
	SB_MAX_ADDR,
};

#define SB_MASK(i)	(1 << i)
#define SB_MASK_ALL	(SB_MASK(SB0_ADDR) | SB_MASK(SB1_ADDR))

/* fsck.c */
struct orphan_info {
	u32 nr_inodes;
	u32 *ino_list;
};

struct extent_info {
	u32 fofs;		/* start offset in a file */
	u32 blk;		/* start block address of the extent */
	u32 len;		/* length of the extent */
};

struct child_info {
	u32 state;
	u32 links;
	u32 files;
	u32 pgofs;
	u8 dots;
	u8 dir_level;
	u32 p_ino;		/* parent ino */
	char p_name[F2FS_NAME_LEN + 1]; /* parent name */
	u32 pp_ino;		/* parent parent ino*/
	struct extent_info ei;
	u32 last_blk;
	u32 i_namelen;  /* dentry namelen */
};

struct f2fs_dentry {
	char name[F2FS_NAME_LEN + 1];
	int depth;
	struct f2fs_dentry *next;
};

struct f2fs_fsck {
	struct f2fs_sb_info sbi;

	struct orphan_info orphani;
	struct chk_result {
		u64 checked_node_cnt;
		u64 valid_blk_cnt;
		u32 valid_nat_entry_cnt;
		u32 valid_node_cnt;
		u32 valid_inode_cnt;
		u32 multi_hard_link_files;
		u64 sit_valid_blocks;
		u32 sit_free_segs;
		u32 wp_fixed;
		u32 wp_inconsistent_zones;
	} chk;

	struct hard_link_node *hard_link_list_head;

	char *main_seg_usage;
	char *main_area_bitmap;
	char *nat_area_bitmap;
	char *sit_area_bitmap;

	u64 main_area_bitmap_sz;
	u32 nat_area_bitmap_sz;
	u32 sit_area_bitmap_sz;

	u64 nr_main_blks;
	u32 nr_nat_entries;

	u32 dentry_depth;
	struct f2fs_dentry *dentry;
	struct f2fs_dentry *dentry_end;
	u32 nat_valid_inode_cnt;
};

#define BLOCK_SZ		4096
struct block {
	unsigned char buf[BLOCK_SZ];
};

enum NODE_TYPE {
	TYPE_INODE = 37,
	TYPE_DIRECT_NODE = 43,
	TYPE_INDIRECT_NODE = 53,
	TYPE_DOUBLE_INDIRECT_NODE = 67,
	TYPE_XATTR = 77
};

struct hard_link_node {
	u32 nid;
	u32 links;
	u32 actual_links;
	struct hard_link_node *next;
};

enum seg_type {
	SEG_TYPE_DATA,
	SEG_TYPE_CUR_DATA,
	SEG_TYPE_NODE,
	SEG_TYPE_CUR_NODE,
	SEG_TYPE_MAX,
};

// F2FS root inode number
#define F2FS_ROOT_INO(sbi)	((sbi)->root_ino_num)

// Checkpoint structure
struct f2fs_checkpoint {
	u32 ckpt_flags;
	// Other fields would be here in a full implementation
};

// Function to get checkpoint
#define F2FS_CKPT(sbi)	((struct f2fs_checkpoint *)(sbi))

// Function to check checkpoint flags
static inline bool is_set_ckpt_flags(struct f2fs_checkpoint *ckpt, unsigned int f)
{
	return ckpt->ckpt_flags & f;
}

static inline bool need_fsync_data_record(struct f2fs_sb_info *sbi)
{
	return !is_set_ckpt_flags(F2FS_CKPT(sbi), CP_UMOUNT_FLAG);
}

// F2FS file types
#define F2FS_FT_UNKNOWN		0
#define F2FS_FT_REG_FILE	1
#define F2FS_FT_DIR		2
#define F2FS_FT_CHRDEV		3
#define F2FS_FT_BLKDEV		4
#define F2FS_FT_FIFO		5
#define F2FS_FT_SOCK		6
#define F2FS_FT_SYMLINK		7
#define F2FS_FT_MAX		8

// Compressed block count structure
#define CHEADER_PGOFS_NONE	0xFFFFFFFF
struct f2fs_compr_blk_cnt {
	u32 cnt;
	u32 cheader_pgofs;
};

// Function declarations that fsck_dir needs
extern int fsck_chk_node_blk(struct f2fs_sb_info *, struct f2fs_inode *, u32,
		enum FILE_TYPE, enum NODE_TYPE, u32 *,
		struct f2fs_compr_blk_cnt *, struct child_info *);
void fsck_chk_checkpoint(struct f2fs_sb_info *sbi);
void pretty_print_filename(const u8 *raw_name, u32 len,
			   char out[F2FS_PRINT_NAMELEN], int enc_name);

// Simplified function declarations for fsck_dir
extern void fsck_init(struct f2fs_sb_info *sbi);
extern void fsck_free(struct f2fs_sb_info *sbi);
extern int f2fs_init_sbi(struct f2fs_sb_info **sbi, const char *dev, 
                        struct f2fs_device_info *devs, int ndevs);
extern void f2fs_free_sbi(struct f2fs_sb_info *sbi);

#endif /* _FSCK_H_ */