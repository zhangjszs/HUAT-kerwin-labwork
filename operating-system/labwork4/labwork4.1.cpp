#include <stdio.h>
#include <stdlib.h>
struct PCB // 先来先服务FCFS
{
    char name[10];     // 进程名
    float arrivetime;  // 到达时间
    float servetime;   // 服务时间
    float finishtime;  // 完成时间
    float roundtime;   // 周转时间
    float daiquantime; // 带权周转时间
};
struct PCB a[50];                                       // 定义进程数组
struct PCB *sortarrivetime(struct PCB a[], int n);      // 声明到达时间冒泡排序函数
void FCFS(struct PCB a[], int n, float *t1, float *t2); // 先来先服务算法

// 按到达时间进行冒泡排序
struct PCB *sortarrivetime(struct PCB a[], int n)
{
    int i, j;
    struct PCB t;
    int flag;               // 标志变量，记录在每一趟冒泡中是否有元素交换，没有交换则结束冒泡
    for (i = 1; i < n; i++) // 外层循环控制比较趟数
    {
        flag = 0;                   // 初始值设置为0
        for (j = 0; j < n - i; j++) // 内存循环控制每一趟的比较次数
        {
            if (a[j].arrivetime > a[j + 1].arrivetime) // 将到达时间短的交换到前边
            {
                t = a[j];
                a[j] = a[j + 1];
                a[j + 1] = t;
                flag = 1; // 有交换，flag置1
            }
        }
        if (flag == 0) // 如果一趟排序中没发生任何交换，则排序结束
        {
            break;
        }
    }
    return a; // 返回排序后进程数组
}

// 先来先服务算法
void FCFS(struct PCB a[], int n, float *t1, float *t2)
{
    int i;
    a[0].finishtime = a[0].arrivetime + a[0].servetime; // 完成时间=到达时间+服务时间
    a[0].roundtime = a[0].finishtime - a[0].arrivetime; // 周转时间=完成时间-到达时间
    a[0].daiquantime = a[0].roundtime / a[0].servetime; // 带权时间=周转时间/服务时间
    for (i = 1; i < n; i++)
    {
        if (a[i].arrivetime < a[i - 1].finishtime) // 当前到达时间在上一个作业结束时间之前
        {
            a[i].finishtime = a[i - 1].finishtime + a[i].servetime; // 完成时间=上一个完成时间+服务时间
            a[i].roundtime = a[i].finishtime - a[i].arrivetime;     // 周转时间=完成时间-到达时间
            a[i].daiquantime = a[i].roundtime / a[i].servetime;     // 带权时间=周转时间/服务时间
        }
        else // 当前到达时间在上一个作业结束时间之后
        {
            a[i].finishtime = a[i].arrivetime + a[i].servetime; // 完成时间=到达时间+服务时间
            a[i].roundtime = a[i].finishtime - a[i].arrivetime; // 周转时间=完成时间-到达时间
            a[i].daiquantime = a[i].roundtime / a[i].servetime; // 带权时间=周转时间/服务时间
        }
    }
    printf("=============================================================\n");
    printf("进程相关信息如下：\n\n");
    printf("进程名    ");
    printf("到达时间  ");
    printf("服务时间  ");
    printf("完成时间  ");
    printf("周转时间  ");
    printf("带权周转时间\n");
    for (i = 0; i < n; i++)
    {
        printf("%-10s", a[i].name);
        printf("%-10.0f", a[i].arrivetime);
        printf("%-10.0f", a[i].servetime);
        printf("%-10.0f", a[i].finishtime);
        printf("%-10.0f", a[i].roundtime);
        printf("%10.2f\n", a[i].daiquantime);
        *t1 += a[i].roundtime;
        *t2 += a[i].daiquantime;
    }
}
int main()
{
    float t1;     // 总周转时间
    float t2;     // 总带权周转时间
    float avr_t1; // 平均周转时间
    float avr_t2; // 平均带权周转时间
    int n, i;
    char select = ' ';    // 选择算法变量标识
    while (select != '2') // 不为退出标识，保持循环
    {
        t1 = 0.0f;
        t2 = 0.0f;
        system("clear");
        printf("\n请选择算法：1.先来先服务算法  2.退出程序\n\n请输入选择:  ");
        scanf("%c", &select);
        if (select == '1') // 先来先服务算法
        {
            printf("\n=====================先来先服务算法FCFS=====================\n\n");
            printf("请输入进程数：");
            scanf("%d", &n);
            for (i = 0; i < n; i++)
            {
                printf("%d 进程名:", i + 1);
                scanf("%s", a[i].name);
                printf("到达时间：");
                scanf("%f", &a[i].arrivetime);
                printf("服务时间：");
                scanf("%f", &a[i].servetime);
            }
            getchar();
            sortarrivetime(a, n); // 按到达时间先后进行冒泡排序
            FCFS(a, n, &t1, &t2); // 先来先服务算法
            avr_t1 = t1 / n;
            avr_t2 = t2 / n;
            printf("\n");
            printf("平均周转时间为：%2.2f\n", avr_t1);
            printf("平均带权周转时间为：%2.2f\n", avr_t2);
            getchar();
        }
        else if (select == '2')
        {
            exit(0);
        }
        else
        {
            printf("please enter right choose!\n");
        }
    }
    return 0;
}
