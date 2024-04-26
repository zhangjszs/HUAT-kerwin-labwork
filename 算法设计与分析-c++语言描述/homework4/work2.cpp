#include <iostream>
#include <vector>

using namespace std;

int matrixChainOrder(const vector<int>& dimensions) {
    int n = dimensions.size() - 1;
    vector<vector<int>> dp(n, vector<int>(n, 0));
    vector<vector<int>> opt(n, vector<int>(n, 0));

    for (int l = 2; l <= n; l++) {
        for (int i = 1; i <= n - l + 1; i++) {
            int j = i + l - 1;
            dp[i-1][j-1] = INT_MAX;
            for (int k = i; k < j; k++) {
                int cost = dp[i-1][k-1] + dp[k][j-1] + dimensions[i-1] * dimensions[k] * dimensions[j];
                if (cost < dp[i-1][j-1]) {
                    dp[i-1][j-1] = cost;
                    opt[i-1][j-1] = k;
                }
            }
        }
    }

    cout << "Optimal Matrix Chain Order: ";
    printOptimalParenthesis(opt, 1, n);
    cout << endl;
    return dp[0][n-1];
}

void printOptimalParenthesis(const vector<vector<int>>& opt, int i, int j) {
    if (i == j) {
        cout << "A" << i;
    } else {
        cout << "(";
        printOptimalParenthesis(opt, i, opt[i-1][j-1]);
        printOptimalParenthesis(opt, opt[i-1][j-1] + 1, j);
        cout << ")";
    }
}

int main() {
    vector<int> dimensions = {45, 8, 40, 25, 10};
    int minCost = matrixChainOrder(dimensions);
    cout << "Minimum cost: " << minCost << endl;
    return 0;
}
/*定义 dp[i][j] 表示计算矩阵 A[i:j+1] 的最优计算次数。
初始化 dp[i][i] = 0(对于只有一个矩阵的情况,不需要计算)。
对于长度为 l 的子问题(即连乘子式包含 l 个矩阵),我们需要枚举分割点 k 并选择最优的分割点。
状态转移方程: dp[i][i+l-1] = min(dp[i][i+l-1], dp[i][k] + dp[k+1][i+l-1] + A[i-1]*A[k]*A[i+l-1])
其中 i <= k < i+l-1。
最终的最优计算次数存储在 dp[1][n-1] 中,其中 n 是矩阵的个数。
计算量分析:

对于长度为 l 的子问题,我们需要枚举 O(l) 个分割点,每个分割点需要进行 O(1) 的计算。
因此总的时间复杂度为 O(n^3),其中 n 是矩阵的个数。*/