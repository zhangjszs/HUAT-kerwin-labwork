/*
 * @Author: kerwin-win zhangjszs@foxmail.com
 * @Date: 2024-04-02
 * @LastEditors: kerwin-win zhangjszs@foxmail.com
 * @LastEditTime: 2024-04-02
 * @FilePath: \HUAT-kerwin-labwork\算法设计与分析-c++语言描述\homework1\work4.cpp
 * @Description: 使用分治法求平面点集中最近点对（最近对问题）
 *
 * 算法：分治（近似 O(n log n)）
 * 时间复杂度：O(n log n)
 * 空间复杂度：O(n)
 */
#include <iostream>
#include <algorithm>

#include <cmath>

using namespace std;

struct Point
{
    int x, y;
};

/**
 * @brief 分治法求最近点对距离（递归实现）
 *
 * @param points 点数组（假设按 x 坐标已排序，或在调用前排序）
 * @param n 点的个数
 * @return double 最近点对的距离
 */
double closestPair(Point points[], int n)
{
    // 如果只有 1 个点,则返回无穷大
    if (n <= 1)
    {
        return INFINITY;
    }

    // 如果只有 2 个点,则返回两点间的距离
    if (n == 2)
    {
        return sqrt((points[0].x - points[1].x) * (points[0].x - points[1].x) +
                    (points[0].y - points[1].y) * (points[0].y - points[1].y));
    }

    // 将点集分成左右两半
    int mid = n / 2;
    double left_min = closestPair(points, mid);
    double right_min = closestPair(points + mid, n - mid);
    double min_dist = min(left_min, right_min);

    // 找出位于分割线两侧 min_dist 距离内的点
    Point strip[n];
    int strip_size = 0;
    for (int i = 0; i < n; i++)
    {
        if (abs(points[i].x - points[mid].x) < min_dist)
        {
            strip[strip_size++] = points[i];
        }
    }

    // 对这些点按 y 坐标排序
    sort(strip, strip + strip_size, [](const Point &a, const Point &b)
         { return a.y < b.y; });

    // 检查这些点,找出最近的点对
    for (int i = 0; i < strip_size; i++)
    {
        for (int j = i + 1; j < strip_size && (strip[j].y - strip[i].y) < min_dist; j++)
        {
            double dist = sqrt((strip[i].x - strip[j].x) * (strip[i].x - strip[j].x) +
                               (strip[i].y - strip[j].y) * (strip[i].y - strip[j].y));
            min_dist = min(min_dist, dist);
        }
    }

    return min_dist;
}

/*
分治过程需要 O(n log n) 的时间,因为需要对点集进行排序。

对于位于分割线两侧 min_dist

距离内的点的检查,需要 O(n) 的时间。

因此,总的时间复杂度为 O(n log n)。

*/
int main()
{
    Point points[] = {{2, 3}, {12, 30}, {40, 50}, {5, 1}, {12, 10}, {3, 4}};
    int n = sizeof(points) / sizeof(points[0]);

    double min_dist = closestPair(points, n);
    cout << "最近的点对为: " << endl;
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            double dist = sqrt((points[i].x - points[j].x) * (points[i].x - points[j].x) +
                               (points[i].y - points[j].y) * (points[i].y - points[j].y));
            if (abs(dist - min_dist) < 1e-6)
            {
                cout << "(" << points[i].x << ", " << points[i].y << "), (" << points[j].x << ", " << points[j].y << ")" << endl;
                cout << "距离: " << min_dist << endl;
            }
        }
    }

    return 0;
}