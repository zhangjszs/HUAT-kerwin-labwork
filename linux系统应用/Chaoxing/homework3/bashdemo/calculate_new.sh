#!/bin/bash

# 定义函数
function add() {
  echo "$(($1 + $2))"
}

function subtract() {
  echo "$(($1 - $2))"
}

function multiply() {
  echo "$(($1 * $2))"
}

function divide() {
  echo "$(($1 / $2))"
}

# 主程序
PS3="Select the operation: "
select opt in add subtract multiply divide quit; do
  case $opt in
    add|subtract|multiply|divide)
      read -p "Enter the first number: " num1
      read -p "Enter the second number: " num2
      result=$( "$opt" $num1 $num2 )
      echo "Result: $result"
      ;;
    quit)
      break
      ;;
    *)
      echo "Invalid option."
      ;;
  esac
done