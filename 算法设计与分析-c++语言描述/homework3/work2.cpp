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