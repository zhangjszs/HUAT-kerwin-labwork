/*
 * @Author: kerwin-win zhangjszs@foxmail.com
 * @Date: 2024-04-02
 * @LastEditors: kerwin-win zhangjszs@foxmail.com
 * @LastEditTime: 2024-04-02
 * @FilePath: \HUAT-kerwin-labwork\算法设计与分析-c++语言描述\homework3\work1.cpp
 * @Description: 0/1 背包的贪心近似（按单位重量收益排序，装满为止）
 *
 * 算法：贪心（按 profit/weight 排序）
 * 时间复杂度：O(n log n)
 * 空间复杂度：O(n)
 */

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Item {
    int weight;
    int profit;
    double profitPerWeight;
};

// 比较函数：按单位重量收益降序
bool cmpProfitPerWeight(const Item& a, const Item& b) {
    return a.profitPerWeight > b.profitPerWeight;
}

/**
 * @brief 贪心策略求近似背包解：按 profit/weight 排序，依次放入
 *
 * @param n 物品数
 * @param M 背包容量
 * @param weights 每个物品重量
 * @param profits 每个物品收益
 * @return int 最大近似收益
 */
int greedySolution(int n, int M, const vector<int>& weights, const vector<int>& profits) {
    vector<Item> items(n);
    for (int i = 0; i < n; i++) {
        items[i].weight = weights[i];
        items[i].profit = profits[i];
        items[i].profitPerWeight = static_cast<double>(profits[i]) / weights[i];
    }

    sort(items.begin(), items.end(), cmpProfitPerWeight);

    int totalWeight = 0;
    int totalProfit = 0;

    for (const auto& item : items) {
        if (totalWeight + item.weight <= M) {
            totalWeight += item.weight;
            totalProfit += item.profit;
        } else {
            break;
        }
    }

    return totalProfit;
}

int main() {
    int n = 7;
    int M = 15;
    vector<int> weights = {2, 3, 5, 7, 1, 4, 1};
    vector<int> profits = {10, 5, 15, 7, 6, 18, 3};

    int maxProfit = greedySolution(n, M, weights, profits);
    cout << "Maximum profit: " << maxProfit << endl;

    return 0;
}

/*
首先,我们需要计算每个物品的单位重量收益,即 profits[i] / weights[i]。这个值越大,意味着这个物品越有价值,我们应该优先把它装入背包。

接下来,我们按照单位重量收益从高到低的顺序对物品进行排序。

然后,我们按照排序后的顺序,依次将物品装入背包,直到背包的剩余容量不足以容纳下一个物品。

在装入物品的过程中,我们不断更新背包的总重量和总收益。*/