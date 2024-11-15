#!/bin/bash

# 欢迎信息
echo "欢迎使用求和程序！"

# 初始化数组
numbers=()

# 读取文件中的 10 个数
for i in {1..10}
do
    # 从标准输入读取一个数
    read number

    # 将读取的数添加到数组中
    numbers+=("$number")
done

# 计算数组中所有数的和
sum=0
for num in "${numbers[@]}"
do
    sum=$((sum + num))
done

# 输出结果
echo "文件中的 10 个数是：${numbers[*]}"
echo "这 10 个数的和是：$sum"