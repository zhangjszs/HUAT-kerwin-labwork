#include <iostream>
#include <fstream>
#include <algorithm>

/*
 * @Author: kerwin-win zhangjszs@foxmail.com
 * @Date: 2024-04-02
 * @LastEditors: kerwin-win zhangjszs@foxmail.com
 * @LastEditTime: 2024-04-02
 * @FilePath: \HUAT-kerwin-labwork\算法设计与分析-c++语言描述\experiment2\labworkdp2.cpp
 * @Description: 区间调度 / 活动选择问题的贪心实现，读取 input.txt 并选择不冲突的活动集合
 *
 * 算法：按结束时间排序后贪心选择
 * 时间复杂度：O(n log n)（排序）
 * 空间复杂度：O(n)
 */

using namespace std;

struct node
{
    int s, f;
    int name;
    bool operator<(const node &b) const
    {
        return f < b.f;
    }
};

/**
 * @brief 按结束时间选择活动的贪心算法
 *
 * @param n 活动数量
 * @param act 活动数组（按结束时间排序后使用）
 * @param A 输出布尔数组，A[i] 表示第 i 个活动是否被选择
 */
void GreedySelector(int n, node *act, bool *A)
{
    A[1] = true;
    int j = 1;
    for (int i = 2; i <= n; i++)
    {
        if (act[i].s >= act[j].f)
        {
            A[i] = true;
            j = i;
        }
        else
        {
            A[i] = false;
        }
    }
}

int main()
{
    int n;
    bool a[100];
    node act[100];

    ifstream infile("input.txt");
    if (!infile.is_open())
    {
        cout << "Error opening the file." << endl;
        return 1;
    }

    infile >> n;
    for (int i = 1; i <= n; i++)
        infile >> act[i].name;
    for (int i = 1; i <= n; i++)
        infile >> act[i].s;
    for (int i = 1; i <= n; i++)
        infile >> act[i].f;

    infile.close();

    sort(act + 1, act + n + 1);

    GreedySelector(n, act, a);

    int totalDays = 0;
    int lastEnd = -1;
    for (int i = 1; i <= n; i++)
    {
        if (a[i])
        {
            int profit = act[i].f - act[i].s;
            totalDays += profit;
            lastEnd = max(lastEnd, act[i].f);
            cout << "Activity " << act[i].name << " start: " << act[i].s << " end: " << act[i].f << " profit: " << profit << endl;
        }
    }

    cout << "Total days used: " << totalDays << endl;
    cout << "Last activity ends at day: " << lastEnd << endl;

    return 0;
}
