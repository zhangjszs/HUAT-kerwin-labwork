#include <iostream>
#include <queue>
#include <vector>

using namespace std;

// 定义边的结构体
struct Edge
{
    int from, to, weight;
};

// 比较函数,用于优先队列
struct cmp
{
    bool operator()(const pair<int, int> &a, const pair<int, int> &b)
    {
        return a.first > b.first;
    }
};

int main()
{
    // 图的邻接表表示
    vector<vector<Edge>> graph = {
        {{0, 1, 2}, {0, 2, 8}, {0, 3, 5}},
        {{1, 2, 3}, {1, 4, 3}},
        {{2, 5, 4}},
        {{3, 5, 6}, {3, 6, 9}},
        {{4, 5, 5}, {4, 6, 7}},
        {{5, 6, 2}}};

    int n = 7;                  // 结点数
    int source = 0, target = 5; // 源点和目标点

    // 初始化 D 和 S 数组
    vector<int> D(n, INT_MAX);
    vector<int> S(n, -1);
    D[source] = 0;

    // 优先队列,存放(距离,结点)
    priority_queue<pair<int, int>, vector<pair<int, int>>, cmp> pq;
    pq.push({0, source});

    while (!pq.empty())
    {
        int u = pq.top().second;
        pq.pop();

        if (u == target)
            break; // 找到目标点,退出

        // 遍历 u 的邻居结点
        for (const auto &edge : graph[u])
        {
            int v = edge.to;
            int weight = edge.weight;
            if (D[v] > D[u] + weight)
            {
                D[v] = D[u] + weight;
                S[v] = u;
                pq.push({D[v], v});
            }
        }
    }

    // 输出最短路径
    cout << "最短路径: ";
    int curr = target;
    while (curr != -1)
    {
        cout << curr << " ";
        curr = S[curr];
    }
    cout << endl;
    cout << "最短路径长度: " << D[target] << endl;

    return 0;
}
/*
优先队列的变化过程：
首先，创建了一个优先队列 pq，用于存放 (距离, 结点) 的 pair 元素。
将源点加入优先队列：pq.push({0, source})。
在每一次迭代中，从优先队列中取出距离最小的结点 u：int u = pq.top().second，然后将其从队列中移除：pq.pop()。
遍历结点 u 的邻居结点，根据 Dijkstra 算法的思想更新最短路径：
如果通过结点 u 到达结点 v 的路径长度 D[v] 大于通过结点 u 的路径长度 D[u] 加上边的权重 weight，则更新 D[v] 和 S[v]，并将结点 v 加入优先队列 pq：pq.push({D[v], v})。
推导过程：
初始化 D 数组和 S 数组，将源点的最短路径长度 D[source] 设为 0。
进入循环，直到优先队列为空：
从优先队列中取出距离最小的结点 u。
如果结点 u 是目标点，退出循环。
遍历结点 u 的邻居结点，对于每条边 (u, v)：
如果通过结点 u 到达结点 v 的路径长度 D[v] 大于通过结点 u 的路径长度 D[u] 加上边的权重 weight，则更新 D[v] 和 S[v]，并将结点 v 加入优先队列 pq。
循环结束后，最短路径的长度存储在 D[target] 中，最短路径上的结点顺序存储在数组 S 中。
最短路径及长度的输出：
输出最短路径：从目标点开始，沿着前驱结点数组 S 逆序输出每个结点。
输出最短路径长度：输出 D[target]。
*/
