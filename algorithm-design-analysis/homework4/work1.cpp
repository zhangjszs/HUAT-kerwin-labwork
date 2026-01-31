#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <algorithm>

using namespace std;

vector<int> dijkstra(vector<vector<int>>& edges, int start, int end)
{
    int n = 0;
    for (auto& edge : edges) {
        n = max(n, max(edge[0], edge[1]));
    }
    n += 1;

    vector<int> dp(n, INT_MAX);
    vector<int> prev(n, -1);
    dp[start] = 0;

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({0, start});

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        if (u == end) {
            break;
        }

        for (auto& edge : edges) {
            int v = edge[1], w = edge[2];
            if (edge[0] == u && dp[v] > dp[u] + w) {
                dp[v] = dp[u] + w;
                prev[v] = u;
                pq.push({dp[v], v});
            }
        }
    }

    vector<int> path;
    int curr = end;
    while (curr != -1) {
        path.push_back(curr);
        curr = prev[curr];
    }
    reverse(path.begin(), path.end());

    return path;
}

int main()
{
    vector<vector<int>> edges = {
        {0, 1, 5},
        {0, 2, 2},
        {1, 3, 3},
        {1, 5, 3},
        {2, 3, 6},
        {2, 4, 5},
        {2, 5, 8},
        {3, 7, 4},
        {4, 6, 6},
        {4, 7, 2},
        {5, 6, 6},
        {5, 7, 2},
        {6, 8, 7},
        {7, 8, 3}
    };

    auto path = dijkstra(edges, 0, 8);
    int minCost = 0;
    for (int i = 1; i < path.size(); i++) {
        for (auto& edge : edges) {
            if (edge[0] == path[i-1] && edge[1] == path[i]) {
                minCost += edge[2];
                break;
            }
        }
    }
    cout << "Minimum cost: " << minCost << endl;
    cout << "Path: ";
    for (int node : path) {
        cout << node << " ";
    }
    cout << endl;

    return 0;
}

/*
求从节点0到节点8的最短路径。我们使用一个 dp 数组来存储从起点到每个节点的最短距离,初始化时 dp[0] = 0，其他元素设为无穷大。
然后我们按照拓扑排序的顺序,依次计算每个节点的最短距离:
处理节点0:
dp[0] = 0
更新 dp[1] = min(dp[1], dp[0] + 5) = 5
更新 dp[2] = min(dp[2], dp[0] + 2) = 2
处理节点1:
dp[1] = 5
更新 dp[3] = min(dp[3], dp[1] + 3) = 8
更新 dp[5] = min(dp[5], dp[1] + 3) = 8
处理节点2:
dp[2] = 2
更新 dp[3] = min(dp[3], dp[2] + 6) = 8
更新 dp[4] = min(dp[4], dp[2] + 5) = 7
更新 dp[5] = min(dp[5], dp[2] + 8) = 10
处理节点3:
dp[3] = 8
更新 dp[7] = min(dp[7], dp[3] + 4) = 12
处理节点4:
dp[4] = 7
更新 dp[6] = min(dp[6], dp[4] + 6) = 13
更新 dp[7] = min(dp[7], dp[4] + 2) = 9
处理节点5:
dp[5] = 8
更新 dp[6] = min(dp[6], dp[5] + 6) = 14
更新 dp[7] = min(dp[7], dp[5] + 2) = 10
处理节点6:
dp[6] = 13
更新 dp[8] = min(dp[8], dp[6] + 7) = 20
处理节点7:
dp[7] = 9
更新 dp[8] = min(dp[8], dp[7] + 3) = 12
最终,我们得到从节点0到节点8的最短距离为12。
通过这个计算过程,我们可以看出动态规划算法是如何逐步更新每个节点的最短距离的。它的时间复杂度是O(|V| + |E|),比Dijkstra算法要快一些。*/