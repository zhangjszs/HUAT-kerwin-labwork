/*
 * @Author: kerwin-win zhangjszs@foxmail.com
 * @Date: 2024-04-14 12:13:29
 * @LastEditors: kerwin-win zhangjszs@foxmail.com
 * @LastEditTime: 2024-04-14 12:13:42
 * @FilePath: \HUAT-kerwin-labwork\算法设计与分析-c++语言描述\experiment1\1.cpp
 * @Description: 循环赛日程表生成算法
 *
 * 算法：分治法生成n个选手循环赛日程表
 * 时间复杂度：O(n^2)
 * 空间复杂度：O(n^2)
 */

#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

/**
 * @brief 打印比赛日程表
 *
 * @param schedule 日程表矩阵
 * @param n 选手数量
 */
void printSchedule(const vector<vector<int>>& schedule, int n) {
    cout << "比赛日程表：" << endl;
    cout << "       ";
    for(int i = 1; i <= n - 1; i++){
        cout << "第" << i << "天 ";
    }
    cout << endl;

    for (int i = 1; i <= n; i++) {
        cout << "选手" << i << ": ";
        for (int j = 2; j <= n; j++) {
            cout << "  " << schedule[i][j] << "   ";
        }
        cout << endl;
    }
}

/**
 * @brief 递归生成比赛日程表
 *
 * 使用分治法将问题分解为子问题
 *
 * @param schedule 日程表矩阵
 * @param n 当前子问题规模
 * @param i 行起始索引
 * @param j 列起始索引
 * @param start 起始天数
 */
void generateSchedule(vector<vector<int>>& schedule, int n, int i, int j, int start) {
    assert(n > 0 && (n & (n - 1)) == 0); // 确保n是2的幂

    if (n == 2) {
        schedule[i][j] = start;
        schedule[i][j + 1] = start + 1;
        schedule[i + 1][j] = start + 1;
        schedule[i + 1][j + 1] = start;
        return;
    }

    int half = n / 2;

    generateSchedule(schedule, half, i, j, start);
    generateSchedule(schedule, half, i, j + half, start + half);
    generateSchedule(schedule, half, i + half, j, start + half);
    generateSchedule(schedule, half, i + half, j + half, start);

    // 调整右下象限的日程表
    for (int row = i + half + 1; row <= i + n - 1; row++) {
        for (int col = j + half + 1; col <= j + n - 1; col++) {
            schedule[row][col] = schedule[row - half][col - half];
        }
    }
}

int main() {
    int k;
    cout << "比赛选手个数为 n (n = 2^k)，请输入参数 k (k > 0): ";
    cin >> k;

    if (k <= 0) {
        cerr << "错误：k必须大于0" << endl;
        return 1;
    }

    int n = 1 << k; // 计算 n = 2^k

    vector<vector<int>> schedule(n + 1, vector<int>(n + 1, 0));

    generateSchedule(schedule, n, 1, 1, 1);

    cout << "参赛人数为: " << n << endl;
    printSchedule(schedule, n);

    return 0;
}