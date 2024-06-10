#include <iostream>
#include <queue>
#include <vector>
#include <limits>
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