#!/bin/bash

# Test script for fsck_dir functionality

echo "=== fsck_dir 功能测试 ==="

# Test 1: Version information
echo "测试 1: 版本信息"
./fsck_dir/fsck_dir -V
echo ""

# Test 2: Basic directory tree output to stdout
echo "测试 2: 基本目录树输出到标准输出"
./fsck_dir/fsck_dir -t /Volumes/1T/f2fs/test_copy.bin | head -10
echo ""

# Test 3: Directory tree output to file
echo "测试 3: 目录树输出到文件"
rm -f test_output.txt
./fsck_dir/fsck_dir -t -o test_output.txt /Volumes/1T/f2fs/test_copy.bin
if [ -f "test_output.txt" ]; then
    echo "✓ 成功生成输出文件"
    echo "文件行数: $(wc -l < test_output.txt)"
    echo "文件大小: $(wc -c < test_output.txt) 字节"
    echo "前5行内容:"
    head -5 test_output.txt
else
    echo "✗ 未能生成输出文件"
fi
echo ""

# Test 4: Verify specific files are present
echo "测试 4: 验证特定文件是否存在"
if grep -q "Gerrit" test_output.txt; then
    echo "✓ 找到 Gerrit 相关条目"
else
    echo "⚠ 未找到 Gerrit 相关条目"
fi

if grep -q "face.jpeg" test_output.txt; then
    echo "✓ 找到 face.jpeg"
else
    echo "⚠ 未找到 face.jpeg"
fi

if grep -q "subdir" test_output.txt; then
    echo "✓ 找到 subdir 目录"
else
    echo "⚠ 未找到 subdir 目录"
fi
echo ""

# Test 5: Error handling
echo "测试 5: 错误处理"
echo "不带 -t 参数运行:"
./fsck_dir/fsck_dir /Volumes/1T/f2fs/test_copy.bin 2>&1 | head -3
echo ""

echo "不带参数运行:"
./fsck_dir/fsck_dir 2>&1 | head -3
echo ""

# Clean up
rm -f test_output.txt

echo "=== 测试完成 ==="