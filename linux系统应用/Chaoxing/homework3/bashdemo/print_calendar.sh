#!/bin/bash

# 获取用户输入的年份和月份
read -p "请输入年份 (例如 2023): " year
read -p "请输入月份 (例如 05): " month

# 使用cal命令打印指定年月的日历
cal $month $year
