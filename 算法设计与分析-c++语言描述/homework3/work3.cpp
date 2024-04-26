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
/*
首先,我们将给定的权重集合 

W = {3, 7, 8, 9, 15, 16, 18, 20, 23, 25, 28}

 按照从小到大的顺序排列。

然后,我们从最小的两个权重开始进行两路合并。在每一步合并中,我们选择两个权重最小的节点进行合并,合并后的新节点的权重就是这两个节点权重之和。

我们不断重复这个过程,直到所有节点都被合并成一棵二叉树。

在合并的过程中,我们记录下每个节点的深度,并计算带权外路径长度。对于每个节点,其在二叉树中的深度乘以该节点的权重,就是这个节点对应的带权外路径长度。最后将所有节点的带权外路径长度相加,就得到了整棵二叉树的带权外路径长度。

根据上述步骤,我们可以构造出最优的两路合并二叉树,如下所示:

    ​    ​478

      /     \

    151     327

   /  \    /  \

  21  130 47  280

 / \  / \  /  /  \

3  18 47 83 18 262*/