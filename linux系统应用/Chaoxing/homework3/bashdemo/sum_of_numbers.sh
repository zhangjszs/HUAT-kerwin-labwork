#!/bin/bash

# 欢迎信息
echo "欢迎使用求和程序！"

# 初始化数组
numbers=()

# 循环读取 5 个数
for i in {1..5}
do
    # 提示用户输入第 i 个数
    echo "请输入第 $i 个数："
    read number

    # 将输入的数添加到数组中
    numbers+=("$number")
done

# 计算数组中所有数的和
sum=0
for num in "${numbers[@]}"
do
    sum=$((sum + num))
done

# 输出结果
echo "输入的 5 个数是：${numbers[*]}"
echo "这 5 个数的和是：$sum"