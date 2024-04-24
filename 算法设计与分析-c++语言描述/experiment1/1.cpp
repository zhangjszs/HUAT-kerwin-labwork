/*
 * @Author: kerwin-win zhangjszs@foxmail.com
 * @Date: 2024-04-14 12:13:29
 * @LastEditors: kerwin-win zhangjszs@foxmail.com
 * @LastEditTime: 2024-04-14 12:13:42
 * @FilePath: \HUAT-kerwin-labwork\算法设计与分析-c++语言描述\experiment1\1.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <iostream>
#include <vector>

using namespace std;

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

void generateSchedule(vector<vector<int>>& schedule, int n, int i, int j, int start) {
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
    int n = 1 << k; // 计算 n = 2^k

    vector<vector<int>> schedule(n + 1, vector<int>(n + 1, 0));

    generateSchedule(schedule, n, 1, 1, 1);

    cout << "参赛人数为: " << n << endl;
    printSchedule(schedule, n);

    return 0;
}