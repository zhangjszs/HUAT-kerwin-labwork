#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Item {
    int weight;
    int profit;
    double profitPerWeight;
};

bool cmpProfitPerWeight(const Item& a, const Item& b) {
    return a.profitPerWeight > b.profitPerWeight;
}

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