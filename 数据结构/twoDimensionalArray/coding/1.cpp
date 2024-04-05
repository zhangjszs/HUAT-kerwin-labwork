#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX 10
#define MAXSIZE 50
void matcreat(int matrix[MAX][MAX], int row, int col)
{ // 随机生成稀疏矩阵
    int i, j;
    int dataij[MAX * MAX / 5];
    int count = 0;
    int rowc = 0, colc = 0;
    do
    {
        srand((int)time(0));
        // count = rand() % (row * col / 5);  // 确定稀疏矩阵非零元个数
        count = 10;
    } while (count < (row * col / 5 - 3)); // 设法使得稀疏数据量接近总量的20%，避免数据量过少
    for (i = 0; i < row; i++)
        for (j = 0; j < col; j++)
            matrix[i][j] = 0;   // 赋初值
    for (i = 0; i < count; i++) // 利用技巧解决了非零元分布的均匀性
    {
        dataij[i] = rand() % 100; // 先产生一个两位数，首位可以为0
        rowc = dataij[i] / 10;    // 十位数约定给行下标
        colc = dataij[i] % 10;    // 个位数约定给列下标
        do
        {
            matrix[rowc][colc] = rand() % 100; // 把这个位置控制住，再产生一个一位数的随机数存入
        } while (matrix[rowc][colc] < 10);
    }
    printf("这是产生的稀疏矩阵：\n");
    for (i = 0; i < row; i++)
    {
        for (j = 0; j < col; j++)
        {
            printf("%6d", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void compress(int matrix[MAX][MAX], int row, int col)
{ // 稀疏矩阵的三元组压缩
    int compress[10][3];
    int i, j;
    //int rowc, colc;
    compress[0][0] = i;
    compress[0][1] = j;
    int index = 1;
    for (row = 0; row < i; row++)
    {
        for (col = 0; col < j; col++)
        {
            int value = matrix[row][col];
            if (value != 0)
            {
                compress[index][0] = row + 1;
                compress[index][1] = col + 1;
                compress[index][2] = value;
                index++;
            }
        }
    }
    compress[0][2] = index - 1;
    printf("压缩结果：\n{");
    for (int k = 1; k <= index; k++)
    {
        // 发现存储的行值为空，说明已经没有非0元素了
        if (compress[k][0] == 0)
        {
            break;
        }
        printf("(%d, %d, %d)", compress[k][0], compress[k][1], compress[k][2]);
        if (k < index - 1)
        {
            printf(",\n");
        }
    }
    printf("}\n");
}
int main()
{
    int row, col;
    int matrixdata[MAX][MAX];
    char choice;
    while (1)
    {
        printf("    稀疏矩阵压缩存储\n");
        system("pause");
        printf("    请输入稀疏矩阵的规模(行，列)：");
        scanf("%d %d", &row, &col);
        matcreat(matrixdata, row, col);
        compress(matrixdata, row, col);
        printf("是否继续？按N结束，其他任意键继续…");
        fflush(stdin);
        scanf("%c", &choice);
        system("cls");
        if (choice == 'N' || choice == 'n')
            break;
    }
    return 0;
}

