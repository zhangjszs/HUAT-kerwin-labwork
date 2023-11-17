/*
 * @Description: 
 * @Version: 2.0
 * @Autor: kerwinz
 * @Date: 2023-11-17 16:09:15
 * @LastEditors: kerwinz
 * @LastEditTime: 2023-11-17 16:23:58
 */
#include <stdio.h>
#include <string.h>

void generateMagicSquare(int n) {
    int magicSquare[n][n];

    // 使用memset函数进行初始化
    memset(magicSquare, 0, sizeof(magicSquare));

    int row = 0, col = n / 2;

    for (int num = 1; num <= n * n; num++) {
        magicSquare[row][col] = num;

        // 计算下一步的位置
        int nextRow = (row - 1 + n) % n;
        int nextCol = (col + 1) % n;

        // 处理重复填充情况
        if (magicSquare[nextRow][nextCol] != 0) {
            nextRow = (row + 1) % n;
            nextCol = col;
        }

        // 更新当前位置
        row = nextRow;
        col = nextCol;
    }

    // 输出魔方阵
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d\t", magicSquare[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int n;
    printf("请输入奇数n：");
    scanf("%d", &n);

    // 检查输入是否为奇数
    if (n % 2 == 0) {
        printf("输入错误，n必须为奇数。\n");
        return 0;
    }

    generateMagicSquare(n);

    return 0;
}