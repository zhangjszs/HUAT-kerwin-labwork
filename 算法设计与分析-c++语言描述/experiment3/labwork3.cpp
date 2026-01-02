#include <iostream>
/*
 * @Author: kerwin-win zhangjszs@foxmail.com
 * @Date: 2024-04-02
 * @LastEditors: kerwin-win zhangjszs@foxmail.com
 * @LastEditTime: 2024-04-02
 * @FilePath: \HUAT-kerwin-labwork\算法设计与分析-c++语言描述\experiment3\labwork3.cpp
 * @Description: 矩阵链乘积的动态规划实现，求最优括号化方式
 *
 * 算法：动态规划
 * 时间复杂度：O(n^3)
 * 空间复杂度：O(n^2)
 */

using namespace std;
const int N = 100;
int p[N] = {10, 20, 25, 15, 5, 10, 25}; // 数据的维数
int m[N][N]; // 最优解
int s[N][N]; // 断点位置

/**
 * @brief 计算矩阵链乘的最小标量乘次数并记录断点
 *
 * @param n 矩阵个数
 */
void MatrixChain(int n) {
    int r, i, j, k;
    for (i = 0; i <= n; i++) // 初始化对角线
        m[i][i] = 0;
    for (r = 2; r <= n; r++) { // r个矩阵连乘
        for (i = 1; i <= n - r + 1; i++) {
            j = i + r - 1;
            m[i][j] = m[i][i] + m[i + 1][j] + p[i - 1] * p[i] * p[j];
            s[i][j] = i;
            for (k = i + 1; k < j; k++) { // 改换分隔位置，逐一测试
                int t = m[i][k] + m[k + 1][j] + p[i - 1] * p[k] * p[j];
                if (t < m[i][j]) { // 如果变换后的位置更优，则替换原来的分隔方法
                    m[i][j] = t; // 记录最少计算次数
                    s[i][j] = k; // 记录断开位置
                }
            }
        }
    }
}

/**
 * @brief 根据 s 数组回溯输出最优括号化方式
 *
 * @param i 左下标
 * @param j 右下标
 */
void Traceback(int i, int j) {
    if (i == j) {
        cout << "A" << i;
    } else {
        cout << "(";
        Traceback(i, s[i][j]);
        Traceback(s[i][j] + 1, j);
        cout << ")";
    }
}

int main() {
    int n = 6; // 矩阵的个数
    MatrixChain(n);
    cout << "最小乘法次数为：" << m[1][n] << endl;
    cout << "最优的括号化方式为：";
    Traceback(1, n);
    cout << endl;

    return 0;
}