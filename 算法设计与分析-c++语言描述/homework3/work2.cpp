#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Job {
    int profit;
    int deadline;
};

bool cmpProfit(const Job& a, const Job& b) {
    return a.profit > b.profit;
}

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