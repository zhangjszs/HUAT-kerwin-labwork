#ifndef EXPRESSION_CALCULATOR_HPP
#define EXPRESSION_CALCULATOR_HPP

#include <string>

class ExpressionCalculator
{
private:
    std::string expr;
    size_t pos;

    // 辅助函数声明
    void skipWhitespace();
    char peek();
    void consume();

    // 解析函数声明
    double parseNumber();
    double parseFactor();
    double parseTerm();
    double parseExpression();

public:
    double calculate(const std::string &expression);
};

// 计算表达式的主函数声明
double evaluateExpression(const std::string &expression);

#endif // EXPRESSION_CALCULATOR_HPP