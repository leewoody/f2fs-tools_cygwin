#include "include/simple_fsck.h"
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

// Global variables
extern char *tree_mark;
extern uint32_t tree_mark_size;

void fsck_usage()
{
	printf("\nUsage: fsck_dir [options] device\n");
	printf("[options]:\n");
	printf("  -t show directory tree\n");
	printf("  -V print the version number and exit\n");
	printf("  -o output file (default: stdout)\n");
	exit(1);
}

static void do_fsck_dir(struct f2fs_sb_info *sbi)
{
	// Traverse the actual filesystem and generate the directory tree
	// This will call into libf2fs_simplified.c to do the real work
	struct f2fs_inode root_inode;
	memset(&root_inode, 0, sizeof(root_inode));
	
	// Initialize child info for root directory
	struct child_info child = {0};
	child.p_ino = F2FS_ROOT_INO_NUM;
	strcpy(child.p_name, "");
	child.pp_ino = F2FS_ROOT_INO_NUM;
	
	// Check the root node
	u32 blk_cnt = 0;
	struct f2fs_compr_blk_cnt cbc = {0};
	fsck_chk_node_blk(sbi, &root_inode, F2FS_ROOT_INO_NUM, F2FS_FT_DIR, 
			 TYPE_INODE, &blk_cnt, &cbc, &child);
}

int main(int argc, char *argv[])
{
	struct f2fs_sb_info *sbi;
	int option = 0;
	int err = 0;
	char *output_filename = NULL;
	char *device_path = NULL;
	int output_fd = -1;

	if (argc < 2) {
		printf("\tError: Device not specified\n");
		fsck_usage();
	}

	// Simple config structure
	struct {
		int show_dentry;
		struct {
			char *path;
		} devices[1];
		int func;
	} c = {0};

	c.func = 0; // FSCK

	// Parse options
	while ((option = getopt(argc, argv, "tVo:")) != EOF) {
		switch (option) {
		case 't':
			c.show_dentry = 1;
			break;
		case 'V':
			show_version("fsck_dir");
			exit(0);
		case 'o':
			output_filename = optarg;
			break;
		default:
			err = 1;
			break;
		}
		if (err)
			break;
	}

	if (err || optind >= argc) {
		printf("\tError: Device not specified\n");
		fsck_usage();
	}

	if (!c.show_dentry) {
		printf("\tError: Please use -t option to show directory tree\n");
		fsck_usage();
	}

	// Store device path
	device_path = strdup(argv[optind]);

	// If output file is specified, redirect stdout to that file
	if (output_filename) {
		// Open the output file
		output_fd = open(output_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (output_fd == -1) {
			printf("Error: Failed to open output file %s\n", output_filename);
			free(device_path);
			return -1;
		}
		
		// Redirect stdout to the file
		if (dup2(output_fd, STDOUT_FILENO) == -1) {
			printf("Error: Failed to redirect output\n");
			close(output_fd);
			free(device_path);
			return -1;
		}
	}

	// Initialize tree mark
	tree_mark_size = 256;
	tree_mark = calloc(tree_mark_size, 1);
	if (!tree_mark) {
		printf("Error: Failed to allocate tree mark\n");
		if (output_fd != -1) {
			close(output_fd);
		}
		free(device_path);
		return -1;
	}

	c.devices[0].path = device_path;

	// Initialize F2FS filesystem
	if (f2fs_init_sbi(&sbi, c.devices[0].path, NULL, 0)) {
		printf("Error: Failed to initialize F2FS filesystem\n");
		free(tree_mark);
		if (output_fd != -1) {
			close(output_fd);
		}
		free(device_path);
		return -1;
	}

	// Initialize fsck
	fsck_init(sbi);

	do_fsck_dir(sbi);

	// Cleanup fsck
	fsck_free(sbi);

	f2fs_free_sbi(sbi);
	free(tree_mark);
	free(device_path);

	// Close output file if it was opened
	if (output_fd != -1) {
		close(output_fd);
	}

	return 0;
}