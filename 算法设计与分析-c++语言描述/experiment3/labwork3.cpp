#include <iostream>
using namespace std;
const int N = 100;
int p[N] = {10, 20, 25, 15, 5, 10, 25}; // 数据的维数
int m[N][N]; // 最优解
int s[N][N]; // 断点位置

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