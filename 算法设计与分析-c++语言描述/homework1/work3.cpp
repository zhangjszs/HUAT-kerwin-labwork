/*
 * @Author: kerwin-win zhangjszs@foxmail.com
 * @Date: 2024-04-02
 * @LastEditors: kerwin-win zhangjszs@foxmail.com
 * @LastEditTime: 2024-04-02
 * @FilePath: \HUAT-kerwin-labwork\算法设计与分析-c++语言描述\homework1\work3.cpp
 * @Description: 分治法实现大整数乘法（简单分割为高低位的递归实现）
 *
 * 算法：分治（类似学校教材中的大整数乘法分解）
 * 时间复杂度：O(n^log2(3)) 可通过 Karatsuba 改进到 O(n^1.585)
 * 空间复杂度：O(n)
 */
#include <iostream>
#include <cmath>

using namespace std;

/**
 * @brief 将两个 n 位整数按高低位分治相乘（要求 n 为 2 的幂）
 *
 * @param a 第一个整数
 * @param b 第二个整数
 * @param n 当前处理的位数
 * @return long long 相乘结果
 */
long long multiplyNumbers(long long a, long long b, int n) {
    // 如果 n 等于 1,直接返回 a * b
    if (n == 1) {
        return a * b;
    }
    
    // 将 a 和 b 分成高位和低位
    int mid = n / 2;
    long long a_high = a / static_cast<long long>(pow(10, mid));
    long long a_low = a % static_cast<long long>(pow(10, mid));
    long long b_high = b / static_cast<long long>(pow(10, mid));
    long long b_low = b % static_cast<long long>(pow(10, mid));
    
    // 递归计算四个子问题
    long long p1 = multiplyNumbers(a_high, b_high, mid);
    long long p2 = multiplyNumbers(a_low, b_low, mid);
    long long p3 = multiplyNumbers(a_high, b_low, mid);
    long long p4 = multiplyNumbers(a_low, b_high, mid);
    
    // 根据分治结果计算最终结果
    return p1 * static_cast<long long>(pow(10, n)) + (p3 + p4) * static_cast<long long>(pow(10, mid)) + p2;
}

int main() {
    long long a = 12345678;
    long long b = 87654321;
    int n = 8;
    cout << "计算机科学与技术 计算机222 章崇文" << endl;
    long long result = multiplyNumbers(a, b, n);
    cout << a << " * " << b << " = " << result << endl;
    
    return 0;
}