/*
 * @Author: kerwin-win zhangjszs@foxmail.com
 * @Date: 2024-04-02
 * @LastEditors: kerwin-win zhangjszs@foxmail.com
 * @LastEditTime: 2024-04-02
 * @FilePath: \HUAT-kerwin-labwork\算法设计与分析-c++语言描述\homework3\work2.cpp
 * @Description: 作业调度问题的贪心实现（按收益排序并安排到最近可用槽）
 *
 * 算法：贪心（按收益排序）
 * 时间复杂度：O(n^2)（简单实现）
 * 空间复杂度：O(n)
 */

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Job {
    int profit;
    int deadline;
};

// 比较函数：按收益降序
bool cmpProfit(const Job& a, const Job& b) {
    return a.profit > b.profit;
}

/**
 * @brief 贪心安排作业以最大化总收益
 *
 * @param n 作业数
 * @param profits 每个作业的收益
 * @param deadlines 每个作业的截止时间（1-based）
 * @return int 最大可获得的总收益
 */
int greedySolution(int n, const vector<int>& profits, const vector<int>& deadlines) {
    vector<Job> jobs(n);
    for (int i = 0; i < n; i++) {
        jobs[i].profit = profits[i];
        jobs[i].deadline = deadlines[i];
    }

    sort(jobs.begin(), jobs.end(), cmpProfit);

    vector<bool> slots(n, false);
    int totalProfit = 0;

    for (const auto& job : jobs) {
        for (int i = job.deadline - 1; i >= 0; i--) {
            if (!slots[i]) {
                slots[i] = true;
                totalProfit += job.profit;
                break;
            }
        }
    }

    return totalProfit;
}

int main() {
    int n = 7;
    vector<int> profits = {3, 5, 20, 18, 1, 6, 30};
    vector<int> deadlines = {1, 3, 4, 3, 2, 1, 2};

    int maxProfit = greedySolution(n, profits, deadlines);
    cout << "Maximum profit: " << maxProfit << endl;

    return 0;
}
/*
首先,我们将所有作业按照收益从高到低进行排序。这是因为我们希望优先完成那些能带来较高收益的作业。

然后,我们遍历排序后的作业列表,依次检查每个作业是否能够在其截止日期之前完成。

如果某个作业能够在其截止日期之前完成,我们就将其安排在距离截止日期最近的时间槽上。这样能够最大限度地利用可用时间,从而获得最高的总收益。

如果某个作业无法在其截止日期之前完成,我们就跳过它,继续处理下一个作业。

通过这种贪心的方式,我们可以得到一个近似最优的调度方案,并计算出最大收益。*/