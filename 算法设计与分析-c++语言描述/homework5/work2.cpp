#include <iostream>
#include <vector>

using namespace std;

vector<vector<int>> result;

void backtrack(int x1, int x2, int x3)
{
    // 检查显式约束
    if (x1 + x2 + x3 != 14)
    {
        return;
    }
    if (x1 <= 0 || x2 <= 0 || x3 <= 0)
    {
        return;
    }

    // 检查隐式约束
    if (x1 + x2 <= x3 || x1 + x3 <= x2 || x2 + x3 <= x1)
    {
        return;
    }

    // 将该三角形添加到结果中
    result.push_back({x1, x2, x3});
}

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