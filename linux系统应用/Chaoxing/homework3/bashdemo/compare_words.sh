#!/bin/bash

# 提示用户输入第一个单词
read -p "请输入第一个单词: " word1

# 提示用户输入第二个单词
read -p "请输入第二个单词: " word2

# 比较两个单词是否相同
if [ "$word1" = "$word2" ]; then
    echo "Two words match"
else
    echo "Two words do not match"
fi

# 结束程序
echo "End of program"
