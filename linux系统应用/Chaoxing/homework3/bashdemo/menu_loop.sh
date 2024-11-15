#!/bin/bash

# 初始化一个变量来控制循环
continue_menu=true

# 主循环
while $continue_menu; do
    # 显示菜单
    echo "请选择一个选项:"
    echo "a. 当前日期和时间"
    echo "b. 当前登录的用户"
    echo "c. 当前工作目录的名称"
    echo "d. 当前工作目录的内容"
    echo "e. 退出程序"
    echo "Enter a, b, c, d, or e:"

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
        e)
            continue_menu=false
            ;;
        *)
            echo "无效的选项"
            ;;
    esac
done

# 结束程序
echo "End of program"
