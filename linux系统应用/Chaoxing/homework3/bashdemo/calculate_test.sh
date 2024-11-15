#!/bin/bash
# Shebang 行，指定脚本使用 Bash Shell 解释执行。

# 欢迎信息
echo Hello!  @_@
# 输出欢迎信息，带有一些表情符号。
echo Welcome to the calculate testing!
# 欢迎用户来到算术测试程序。
echo You can input an expression such as 2*2 or 3+1, and input the answer
# 提示用户可以输入的表达式类型。
echo I will tell you whether you are right or wrong.
# 告知用户程序会判断他们的答案是否正确。
echo You can input 'q' to exit.
# 提示用户可以输入 'q' 退出程序。
echo "Now let's begin!"
# 表示程序即将开始。

# 初始化变量
number1=0
# 初始化一个变量 `number1`，并将其设置为 `0`。这个变量将用于存储用户输入的第一个数字。

# 主循环，直到用户输入 'q'
while [ "$number1" != "q" ]
do
    # 提示用户输入第一个数字
    echo Input the first number:
    # 提示用户输入第一个数字。
    read number1
    # 从用户那里读取输入，并将其存储在变量 `number1` 中。

    # 如果用户输入 'q'，退出循环
    if [ "$number1" = "q" ]; then
        # 如果用户输入的 `number1` 等于 `q`，则跳出 `while` 循环，结束程序。
        break
    fi

    # 提示用户输入运算符
    echo Input the operator:
    # 提示用户输入运算符。
    read oper
    # 从用户那里读取输入，并将其存储在变量 `oper` 中。

    # 提示用户输入第二个数字
    echo Input the second number:
    # 提示用户输入第二个数字。
    read number2
    # 从用户那里读取输入，并将其存储在变量 `number2` 中。

    # 提示用户输入答案
    echo Input the answer:
    # 提示用户输入答案。
    read yourAnswer
    # 从用户那里读取输入，并将其存储在变量 `yourAnswer` 中。

    # 根据运算符计算正确答案
    case $oper in
        +)  myAnswer=`expr $number1 + $number2`;;
        # 如果运算符是 `+`，使用 `expr` 命令计算加法结果，并将其存储在变量 `myAnswer` 中。
        -)  myAnswer=`expr $number1 - $number2`;;
        # 如果运算符是 `-`，使用 `expr` 命令计算减法结果，并将其存储在变量 `myAnswer` 中。
        \*) myAnswer=`expr $number1 \* $number2`;;
        # 如果运算符是 `*`，使用 `expr` 命令计算乘法结果，并将其存储在变量 `myAnswer` 中。注意 `*` 需要用反斜杠 `\` 转义。
        /) 
            if [ $number2 -eq 0 ]
            then
                echo "Sorry! :-("
                echo "0 cannot be the divisor"
                continue
            else
                myAnswer=`expr $number1 / $number2`
            fi
            ;;
        # 如果运算符是 `/`，首先检查除数 `number2` 是否为 `0`。如果是 `0`，输出错误信息并跳过本次循环；否则，使用 `expr` 命令计算除法结果，并将其存储在变量 `myAnswer` 中。
        *) echo "Error!";;
        # 如果输入的运算符不是 `+`、`-`、`*` 或 `/`，输出错误信息。
    esac

    # 比较用户输入的答案和正确答案
    if [ $myAnswer -eq $yourAnswer ]
    then
        echo ":-)" Congratulations!
        echo Your are right!
    else
        echo ":-(" Sorry!
        echo You are wrong!
        echo "The right answer is:"
        echo "$number1 $oper $number2 = $myAnswer"
    fi
    # 使用 `if` 语句检查用户输入的答案 `yourAnswer` 是否等于正确的答案 `myAnswer`。
    # 如果答案正确，输出恭喜信息；如果答案不正确，输出错误信息和正确的答案。

    # 提示用户是否继续
    echo "Enter the q key to exit, continue with other keys"
    # 提示用户输入 `q` 退出程序，输入其他键继续。
    read answer
    # 从用户那里读取输入，并将其存储在变量 `answer` 中。

    # 如果用户输入 'q'，退出循环
    if [[ $answer = q ]]
    then
        # 如果用户输入的 `answer` 等于 `q`，则跳出 `while` 循环，结束程序。
        break
    else
        # 如果用户输入了其他键，继续下一次循环。
        continue
    fi
done

# 结束信息
echo "End of program"
# 输出程序结束信息。