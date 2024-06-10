#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

unordered_map<int, vector<int>> adjList;
int m = 3;   // 假设色数为3
int sum = 0; // 符合条件的着色方案数量

char getColorCode(int color)
{
    switch (color)
    {
    case 1:
        return 'A';
    case 2:
        return 'B';
    case 3:
        return 'C';
    default:
        return '?';
    }
}

// 判断当前点的着色是否合法
bool isValidColoring(vector<int> &colors, int node, int color)
{
    for (int neighbor : adjList[node])
    {
        if (colors[neighbor] == color)
            return false;
    }
    return true;
}

// 回溯函数
void backtrack(int node, vector<int> &colors)
{
    if (node == colors.size())
    {
        sum++;
        // 打印当前的着色方案
        for (int i = 0; i < colors.size(); ++i)
        {
            cout << getColorCode(colors[i]);
        }
        cout << endl;
        return;
    }
    for (int c = 1; c <= m; ++c)
    {
        if (isValidColoring(colors, node, c))
        {
            colors[node] = c;
            backtrack(node + 1, colors);
            colors[node] = 0; // 回溯，重置当前节点颜色
        }
    }
}

int main()
{
    // 直接初始化图结构
    adjList = {
        {1, {2, 4, 0}}, // 注意调整索引以匹配点的实际编号
        {2, {1, 3, 0}},
        {3, {2, 4}},
        {4, {1, 3, 0}},
        {0, {1, 2, 4}} // 确保所有点都被初始化
    };
    int pointnum = adjList.size();
    vector<int> colors(pointnum, 0);
    backtrack(0, colors); // 从第一个点（索引为0）开始搜索
    cout << "一共有 " << sum << " 种绘色方案" << endl;
    return 0;
}