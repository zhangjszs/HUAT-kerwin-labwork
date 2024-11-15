#!/bin/bash

# 欢迎信息
echo "欢迎使用日历打印程序！"

# 检查是否提供了两个参数
if [ $# -ne 2 ]; then
    echo "Usage: $0 <year> <month>"
    echo "Example: $0 2023 05"
    exit 1
fi

# 获取年份和月份
year=$1
month=$2

# 使用 cal 命令打印指定年月的日历
cal $month $year