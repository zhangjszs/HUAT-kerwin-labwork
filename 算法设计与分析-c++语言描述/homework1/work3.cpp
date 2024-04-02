/*
用分治法实现两个n位整数的乘法。为简单请见，假设n等于2的k次幂，例如n=8，同时假定n位整数的大小在C语言整数的运算范围内, 
设计算法并调试计算12345678*87654321，并分析算法的时间复杂度。（25分）
*/
#include <iostream>
#include <cmath>

using namespace std;

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