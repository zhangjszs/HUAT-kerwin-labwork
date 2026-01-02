#include <iostream>
#include <queue>
#include <vector>
#include <limits>

/*
 * @Author: kerwin-win zhangjszs@foxmail.com
 * @Date: 2024-04-02
 * @LastEditors: kerwin-win zhangjszs@foxmail.com
 * @LastEditTime: 2024-04-02
 * @FilePath: \HUAT-kerwin-labwork\算法设计与分析-c++语言描述\experiment5\labwork5.cpp
 * @Description: Dijkstra 最短路径示例，使用优先队列实现
 *
 * 算法：Dijkstra（用最小堆模拟优先队列）
 * 时间复杂度：O(E log V)
 * 空间复杂度：O(V)
 */

using namespace std;

const int INF = numeric_limits<int>::max();
vector<vector<int>> graph = {
    {INF, 2, 8, 5, INF, INF, INF},
    {INF, INF, 3, INF, 3, INF, INF},
    {INF, INF, INF, INF, INF, 4, INF},
    {INF, INF, INF, INF, INF, 6, 9},
    {INF, INF, INF, INF, INF, 5, 7},
    {INF, INF, INF, INF, INF, INF, 2},
};

vector<int> distances(7, INF);
vector<int> parent(7, -1);

struct Compare {
    bool operator()(const pair<int, int>& a, const pair<int, int>& b) {
        return a.second > b.second; 
    }
};

/**
 * @brief 使用优先队列实现 Dijkstra 算法
 *
 * @param start 起点索引
 * @param end 目标点索引（可用于提前退出）
 * @return true 找到路径
 * @return false 无法到达
 */
bool dijkstra(int start, int end) {
    priority_queue<pair<int, int>, vector<pair<int, int>>, Compare> pq;
    distances[start] = 0;
    pq.push({start, 0});

    while (!pq.empty()) {
        int u = pq.top().first;
        pq.pop();

        if (u == end) return true;

        for (int v = 0; v < 7; ++v) {
            if (graph[u][v] != INF && distances[u] + graph[u][v] < distances[v]) {
                distances[v] = distances[u] + graph[u][v];
                parent[v] = u;
                pq.push({v, distances[v]});
            }
        }
    }

    return false; // 无法到达
}

/**
 * @brief 打印从起点到终点的路径（通过 parent 数组回溯）
 *
 * @param end 终点索引
 */
void printPath(int end) {
    if (distances[end] == INF) {
        cout << "没有找到路径\n";
        return;
    }
    int current = end;
    while (current != -1) {
        cout << current << " <- ";
        current = parent[current];
    }
    cout << "0\n";
}

int main() {
    if (dijkstra(0, 6)) {
        cout << "最短路径长度" << distances[6] << "\n";
        printPath(6);
    } else {
        cout << "没有到6的路径" ;
    }
    return 0;
}