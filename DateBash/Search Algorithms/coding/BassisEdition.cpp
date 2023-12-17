// 运用非递归方式设计折半查找法的程序
// 程序构思：                            /
// 假设middle=n/2，此时左边办left=0，右边界right=0。
// 如果KeyValue<Data[middle]:
// 　　表示KeyValue可能出现在Data[middle]之前，所以查找Data[0]到Data[middle-1]之间的数据。
// 　　这时left=left，right=right-1，而middle=(left+right)/2。
// 如果KeyValue>Data[middle]:
// 　　表示KeyValue可能出现在Data[middle]之后，所以查找Data[middle+1]到Data[n]之间的数据。
// 　　这时left=middle+1，right=right，而middle=(left+right)/2。
// 如果KeyValue=Data[middle]:
// 　　表示已查找到数据。
// 重复执行上述3个步骤直到left=right或者找到欲查找数据为止。

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#define MAX 100
int Counter = 1; // 计数器

void produce_data(int data[], int num) // 随机产生一批数
{
    int i;
    srand((unsigned)time(NULL));
    for (i = 0; i < num; i++)
        data[i] = rand() % 100;
}

void print_data(int data[], int num) // 将随机产生的一批数排序输出
{
    int i, j, count = 0; // 排序采用的是冒泡排序
    int change = 0;      // 记录数值是否有交换位置*/
    int temp;            // 数值交换时的暂存变量

    while (!change)
    {
        change = 1;
        for (j = num; j > 0; j--) // 外层循环
        {
            for (i = 0; i < j - 1; i++) // 内层循环
            {
                if (data[i] > data[i + 1]) // 前者较后者大
                {
                    // 两数值交换位置
                    temp = data[i];
                    data[i] = data[i + 1];
                    data[i + 1] = temp;
                    change = 0;
                }
            }
        }
    }
    for (i = 0; i < num; i++)
    {
        printf("%5d", data[i]);
        count++;
        if (count % 5 == 0)
            printf("\n");
    }
}

int Binary_Search(int data[], int Key, int num)
{               // 折半查找法
    int Left;   // 左边界变量
    int Right;  // 右边界变量
    int Middle; // 中位数变量

    Left = 0;
    Right = num - 1;

    while (Left <= Right)
    {
        Middle = (Left + Right) / 2;
        if (Key < data[Middle]) // 欲查找值较小
        {
            Right = Middle - 1; // 查找前半段
        }
        else if (Key > data[Middle]) // 欲查找值较大
        {
            Left = Middle + 1; // 查找后半段
        }
        else if (Key == data[Middle]) // 查找到数据
        {
            printf("data[%d]=%d\n", Middle, data[Middle]);
            return 1;
        }
        Counter++;
    }
    return 0;
}

int main()
{
    int DATA[MAX]; // 原始数据
    int KeyValue;  // 欲查找数据变量
    int num;
    char choice;
    
    while (1)
    {
        printf("请输入需要产生的数据个数:");
        scanf("%d", &num);
        produce_data(DATA, num);
        
        printf("产生的这批数据分别为:\n");
        print_data(DATA, num);
        printf("\n");
        5
        printf("请输入你要查找的值:");
        scanf("%d", &KeyValue);
        
        int result = Binary_Search(DATA, KeyValue, num);
        
        if (result)
            printf("查找成功，查找次数=%d。\n", Counter); // 输出查找次数
        else
            printf("数据不存在，查找失败！\n"); // 输出没有找到数据
        
        printf("是否继续(y/n)？\n");
        choice = getch();
        
        if (choice == 'N' || choice == 'n')
            break;
        else
        {
            system("cls");
            fflush(stdin); // 清除键盘缓冲区
        }
        
        Counter = 1;
    }
    
    return 0;
}
