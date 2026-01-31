#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 1000

void bubbleSort(int *list, int n)
{
    int i, j, temp;
    for (i = 0; i < n - 1; i++)
    {
        for (j = 0; j < n - i - 1; j++)
        {
            if (list[j] > list[j + 1])
            {
                temp = list[j];
                list[j] = list[j + 1];
                list[j + 1] = temp;
            }
        }
    }
}

void quickSort(int *list, int left, int right)
{
    if (left < right)
    {
        int i = left, j = right, pivot = list[left];
        while (i < j)
        {
            while (i < j && list[j] >= pivot)
                j--;
            if (i < j)
                list[i++] = list[j];
            while (i < j && list[i] < pivot)
                i++;
            if (i < j)
                list[j--] = list[i];
        }
        list[i] = pivot;
        quickSort(list, left, i - 1);
        quickSort(list, i + 1, right);
    }
}

void heapify(int *heap, int parent, int size)
{
    int temp = heap[parent];
    int child = 2 * parent + 1;

    while (child < size)
    {
        if (child + 1 < size && heap[child] < heap[child + 1])
        {
            child++;
        }
        if (temp >= heap[child])
        {
            break;
        }
        heap[parent] = heap[child];
        parent = child;
        child = 2 * parent + 1;
    }

    heap[parent] = temp;
}

void heapSort(int *heap, int size)
{
    int i, temp;
    for (i = size / 2 - 1; i >= 0; i--)
    {
        heapify(heap, i, size);
    }
    for (i = size - 1; i > 0; i--)
    {
        temp = heap[0];
        heap[0] = heap[i];
        heap[i] = temp;
        heapify(heap, 0, i);
    }
}

void produceData(int data[], int num)
{
    int i;
    srand((unsigned)time(NULL));
    for (i = 0; i < num; i++)
    {
        data[i] = rand() % 100;
    }
}

void produceData1(int data[], int num)
{
    int i;
    srand((unsigned)time(NULL));
    for (i = 1; i <= num; i++)
    {
        data[i] = rand() % 100;
    }
}

void printData(int data[], int num)
{
    int i;
    int count = 0;
    for (i = 0; i < num; i++)
    {
        printf("%5d", data[i]);
        count++;
        if (count % 10 == 0)
            printf("\n");
    }
    printf("\n");
}

void printData1(int data[], int num)
{
    int i;
    int count = 0;
    for (i = 1; i <= num; i++)
    {
        printf("%5d", data[i]);
        count++;
        if (count % 10 == 0)
            printf("\n");
    }
    printf("\n");
}

void showMenu()
{
    printf("    欢迎使用数据排序小软件\n");
    printf("\t1、冒泡排序\n");
    printf("\t2、快速排序\n");
    printf("\t3、堆排序\n");
    printf("\t4、退出程序\n");
}

int main()
{
    int *list;  // 使用指针存储数据
    int i, num; // 数组索引
    int node;   // 读入输入值所使用的暂存变量
    int choice;

    clock_t start, end;
    double cpu_time_used;

    while (1)
    {
        showMenu();
        printf("请输入你的选择：");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("请输入需要排序的元素个数：");
            scanf("%d", &num);

            // 动态分配内存
            list = (int *)malloc(num * sizeof(int));
            if (list == NULL)
            {
                printf("内存分配失败\n");
                return 1;
            }

            produceData(list, num);
            printf("排序前的数据为:\n");
            printData(list, num);
            start = clock();
            bubbleSort(list, num);
            end = clock();
            cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
            printf("\n最终的排序结果为:\n");
            printData(list, num);
            printf("\n");

            // 释放动态分配的内存
            free(list);
            printf("程序运行时间为：%lf秒\n", cpu_time_used);
            system("pause");
            system("cls");
            break;

        case 2:
            printf("请输入需要排序的元素个数：");
            scanf("%d", &num);

            // 动态分配内存
            list = (int *)malloc(num * sizeof(int));
            if (list == NULL)
            {
                printf("内存分配失败\n");
                return 1;
            }

            produceData(list, num);
            printf("排序前的数据为:\n");
            printData(list, num);

            printf("\n排序过程如下：\n");
            start = clock();
            quickSort(list, 0, num - 1);
            end = clock();
            cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

            printf("\n最终的排序结果为:\n");
            printData(list, num);
            printf("\n");

            // 释放动态分配的内存
            free(list);

            printf("程序运行时间为：%lf秒\n", cpu_time_used);
            system("pause");
            system("cls");
            break;

        case 3:
            printf("请输入需要排序的元素个数：");
            scanf("%d", &num);

            // 动态分配内存
            list = (int *)malloc((num + 1) * sizeof(int));
            if (list == NULL)
            {
                printf("内存分配失败\n");
                return 1;
            }

            produceData1(list, num);
            printf("排序前的数据为:\n");
            printData1(list, num);

            printf("\n排序过程如下：\n");
            start = clock();
            heapSort(list, num);
            end = clock();
            cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

            printf("\n最终的排序结果为:\n");
            printData1(list, num);
            printf("\n");

            // 释放动态分配的内存
            free(list);

            printf("程序运行时间为：%lf秒\n", cpu_time_used);
            system("pause");
            system("cls");
            break;

        case 4:
            printf("谢谢使用，再见！\n");
            return 0;

        default:
            printf("你的选择有误，请重新输入！\n");
        }
    }
}