#include "ExpressionCalculator.hpp"
#include <stdexcept>
#include <iostream>
#include <cctype>

// 跳过空白字符
void ExpressionCalculator::skipWhitespace() {
    while (pos < expr.length() && isspace(expr[pos])) {
        pos++;
    }
}

// 获取当前位置的字符，并跳过空白字符
char ExpressionCalculator::peek() {
    skipWhitespace(); // 跳过空白字符
    if (pos >= expr.length()) {
        return '\0'; // 遇到字符串末尾
    }
    return expr[pos]; // 返回当前字符
}

// 移动指针到下一个字符
void ExpressionCalculator::consume() {
    pos++; // 移动指针
}

// 解析数字
double ExpressionCalculator::parseNumber() {
    skipWhitespace();
    if (!isdigit(peek())) {
        throw std::runtime_error("预期是数字");
    }

    double result = 0;
    while (pos < expr.length() && isdigit(expr[pos])) {
        result = result * 10 + (expr[pos] - '0'); // 将字符转换为数字
        consume(); // 移动指针
    }
    return result;
}

// 解析因子（数字或括号内的表达式）
double ExpressionCalculator::parseFactor() {
    skipWhitespace();
    char ch = peek(); // 获取当前字符

    if (ch == '(') {
        consume(); // 消耗左括号
        double result = parseExpression(); // 递归解析括号内的表达式

        if (peek() != ')') {
            throw std::runtime_error("缺少右括号");
        }
        consume(); // 消耗右括号
        return result;
    }

    return parseNumber();
}

// 解析项（因子乘除运算）
double ExpressionCalculator::parseTerm() {
    double result = parseFactor(); // 解析第一个因子

    while (true) {
        char op = peek();
        if (op != '*' && op != '/') { // 判断是否是乘除运算符
            break;
        }
        consume();

        double factor = parseFactor();
        if (op == '*') {
            result *= factor; // 乘法运算
        } else {
            if (factor == 0) {
                throw std::runtime_error("除数不能为零");
            }
            result /= factor; // 除法运算
        }
    }

    return result;
}

// 解析表达式（项加减运算）
double ExpressionCalculator::parseExpression() {
    double result = parseTerm();

    while (true) {
        char op = peek();
        if (op != '+' && op != '-') { // 判断是否是加减运算符
            break;
        }
        consume();

        double term = parseTerm();
        if (op == '+') {
            result += term;
        } else {
            result -= term;
        }
    }

    return result;
}

// 计算表达式的值
double ExpressionCalculator::calculate(const std::string& expression) {
    expr = expression;
    pos = 0;

    double result = parseExpression(); // 解析表达式

    // 检查是否还有未处理的字符
    skipWhitespace();
    if (pos < expr.length()) {
        throw std::runtime_error("表达式末尾有无效字符");
    }

    return result;
}

// 外部接口，用于计算表达式
double evaluateExpression(const std::string& expression) {
    try {
        ExpressionCalculator calculator;
        return calculator.calculate(expression);
    } catch (const std::runtime_error& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        throw; // 重新抛出异常
    }
}