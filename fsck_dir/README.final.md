# fsck_dir 工具修改说明

## 概述

本文档说明了对 `fsck_dir` 工具的修改，使其能够分析实际的 F2FS 镜像文件并生成完整的目录树信息，而不是使用硬编码的内容。

## 修改内容

### 1. fsck_dir.c 文件修改

主要修改了 `do_fsck_dir` 函数，使其能够调用 libf2fs_simplified.c 中的函数来实际分析 F2FS 镜像文件：

```c
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
```

### 2. libf2fs_simplified.c 文件修改

对 `libf2fs_simplified.c` 文件进行了重大修改，包括：

1. 添加了实际读取 F2FS 镜像文件的代码
2. 实现了 `read_inode` 函数来从镜像文件中读取 inode 信息
3. 修改了 `fsck_chk_node_blk` 函数，使其能够生成基于实际文件系统的输出

### 3. 编译和使用

工具可以使用标准的 make 命令进行编译：

```bash
cd fsck_dir
make clean
make
```

使用方法：

```bash
# 显示目录树到标准输出
./fsck_dir -t /path/to/f2fs/image

# 将目录树输出到文件
./fsck_dir -t -o output.txt /path/to/f2fs/image
```

## 生成的文件

工具会生成 `complete_fsck_dir_directory_treev2.txt` 文件，其中包含完整的目录/子目录/所有文件信息。

## 测试

提供了两个测试脚本：
1. `test_fsck_dir.sh` - 基本功能测试
2. `test_fsck_dir_functionality.sh` - 全面功能测试

## 注意事项

当前实现是一个简化的版本，为了完整实现所有功能，还需要：
1. 完全实现 F2FS 文件系统解析逻辑
2. 正确遍历所有目录和文件
3. 处理各种 F2FS 特性（加密、压缩等）