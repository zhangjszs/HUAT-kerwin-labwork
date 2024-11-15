#!/bin/bash

# 初始化一个变量来存储总和
sum=0

# 只要还有位置参数，就一直循环
while [[ $# -gt 0 ]]; do
  # 将当前参数转换为数字并加到总和上
  sum=$(echo "$sum + $1" | bc)
  # 移动到下一个参数
  shift
done

# 输出计算结果
echo "所有参数的和为：$sum"