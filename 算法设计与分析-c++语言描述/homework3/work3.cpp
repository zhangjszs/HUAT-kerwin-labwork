#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Node {
    int weight;
    int depth;
};

bool cmpWeight(const Node& a, const Node& b) {
    return a.weight < b.weight;
}

int greedySolution(const vector<int>& W) {
    vector<Node> nodes;
    for (int w : W) {
        nodes.push_back({w, 0});
    }
    sort(nodes.begin(), nodes.end(), cmpWeight);

    while (nodes.size() > 1) {
        Node left = nodes.front();
        nodes.erase(nodes.begin());
        Node right = nodes.front();
        nodes.erase(nodes.begin());
        Node merged = {left.weight + right.weight, max(left.depth, right.depth) + 1};
        nodes.push_back(merged);
        sort(nodes.begin(), nodes.end(), cmpWeight);
    }

    int weightedPathLength = 0;
    for (const auto& node : nodes) {
        weightedPathLength += node.weight * node.depth;
    }
    return weightedPathLength;
}

int main() {
    vector<int> W = {3, 7, 8, 9, 15, 16, 18, 20, 23, 25, 28};
    int weightedPathLength = greedySolution(W);
    cout << "Weighted path length: " << weightedPathLength << endl;
    return 0;
}