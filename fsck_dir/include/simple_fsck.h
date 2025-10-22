/**
 * simple_fsck.h
 *
 * Simplified fsck definitions for fsck_dir tool
 */
#ifndef _SIMPLE_FSCK_H_
#define _SIMPLE_FSCK_H_

#include "simple_f2fs.h"

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

/* fsck.c simplified structures */
struct orphan_info {
	u32 nr_inodes;
	u32 *ino_list;
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

struct hard_link_node {
	u32 nid;
	u32 links;
	u32 actual_links;
	struct hard_link_node *next;
};

#endif /* _SIMPLE_FSCK_H_ */