# fsck_dir - 完全独立的 F2FS 目录树查看工具

## 概述

fsck_dir 是一个完全独立的 F2FS 文件系统目录树查看工具，它是从完整的 F2FS 工具集中提取并简化而来的。该工具专注于单一功能：显示 F2FS 镜像文件的目录树结构。

与原版 fsck.f2fs 工具不同，这个版本：

1. **完全独立**：不依赖主 F2FS 工具集的任何库文件
2. **轻量级**：只包含显示目录树所需的最小功能集
3. **易于构建**：只需要标准 C 编译器即可构建
4. **易于分发**：所有必要文件都包含在单个文件夹中

## 文件结构

```
fsck_dir/
├── fsck_dir.c              # 主程序源代码
├── libf2fs_simplified.c    # 简化的 F2FS 库函数实现
├── Makefile                # 构建配置文件
├── README.md               # 使用说明
├── README.final.md         # 此文件
├── test_fsck_dir.sh        # 测试脚本
└── include/                # 头文件目录
    ├── simple_f2fs.h       # 简化的 F2FS 定义
    └── simple_fsck.h       # 简化的 FSCK 定义
```

## 构建要求

- 标准 C 编译器 (GCC 或 Clang)
- Make 构建工具

## 构建方法

```bash
cd fsck_dir
make
```

## 使用方法

```bash
# 显示 F2FS 镜像的目录树
./fsck_dir -t /path/to/f2fs_image

# 显示版本信息
./fsck_dir -V

# 显示帮助信息
./fsck_dir
```

## 功能特点

- 仅实现目录树显示功能 (`-t` 参数)
- 移除了所有文件系统修复和验证功能
- 移除了所有复杂的依赖关系
- 简化了数据结构和函数实现
- 保留了与原版工具相同的命令行接口

## 输出示例

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

## 设计理念

这个工具的设计遵循以下原则：

1. **单一职责**：只做一件事，并做好
2. **独立性**：不依赖外部库或主项目
3. **简化性**：移除所有不必要的复杂性
4. **兼容性**：保持与原版工具相同的用户接口

## 与原版工具的区别

| 特性 | 原版 fsck.f2fs | 独立版 fsck_dir |
|------|----------------|-----------------|
| 文件大小 | 大 | 小 |
| 依赖关系 | 复杂 | 无 |
| 功能范围 | 全面 | 单一 |
| 构建难度 | 高 | 低 |
| 分发便利性 | 低 | 高 |

## 许可证

该工具遵循与原版 F2FS 工具相同的许可证（GPL v2）。