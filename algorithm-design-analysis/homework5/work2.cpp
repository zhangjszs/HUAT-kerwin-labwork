/*
 * @Author: kerwin-win zhangjszs@foxmail.com
 * @Date: 2024-04-02
 * @LastEditors: kerwin-win zhangjszs@foxmail.com
 * @LastEditTime: 2024-04-02
 * @FilePath: \HUAT-kerwin-labwork\算法设计与分析-c++语言描述\homework5\work2.cpp
 * @Description: 通过回溯枚举满足边长和为14的三角形边长组合并检查三角形不等式
 *
 * 算法：穷举 + 约束过滤
 * 时间复杂度：O(n^2)
 * 空间复杂度：O(n)
 */

#include <iostream>
#include <vector>

using namespace std;

vector<vector<int>> result;

// 检查并保存满足条件的三角形三边组合
void backtrack(int x1, int x2, int x3)
{
    // 显式约束：三边和为14
    if (x1 + x2 + x3 != 14)
    {
        return;
    }
    if (x1 <= 0 || x2 <= 0 || x3 <= 0)
    {
        return;
    }

    // 隐式约束：三角形不等式
    if (x1 + x2 <= x3 || x1 + x3 <= x2 || x2 + x3 <= x1)
    {
        return;
    }

    result.push_back({x1, x2, x3});
}

// 枚举所有可能的 (x1,x2) 并由和推导出 x3
void solve()
{
    for (int i = 1; i <= 13; i++)
    {
        for (int j = 1; j <= 13 - i; j++)
        {
            int x3 = 14 - i - j;
            backtrack(i, j, x3);
        }
    }
}

int main()
{
    solve();

    cout << "所有可能的三角形边长组合如下:" << endl;
    for (const auto &triangle : result)
    {
        cout << "x1 = " << triangle[0] << ", x2 = " << triangle[1] << ", x3 = " << triangle[2] << endl;
    }

    cout << "共有 " << result.size() << " 种不同的三角形。" << endl;

    return 0;
}