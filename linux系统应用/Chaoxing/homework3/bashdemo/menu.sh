#!/bin/bash

# 显示菜单
echo "请选择一个选项:"
echo "a. 当前日期和时间"
echo "b. 当前登录的用户"
echo "c. 当前工作目录的名称"
echo "d. 当前工作目录的内容"
echo "Enter a, b, c, or d:"

# 读取用户输入
read choice

# 根据用户输入执行相应的操作
case $choice in
    a)
        echo "当前日期和时间:"
        date
        ;;
    b)
        echo "当前登录的用户:"
        whoami
        ;;
    c)
        echo "当前工作目录的名称:"
        pwd
        ;;
    d)
        echo "当前工作目录的内容:"
        ls
        ;;
    *)
        echo "无效的选项"
        ;;
esac

# 结束程序
echo "End of program"
