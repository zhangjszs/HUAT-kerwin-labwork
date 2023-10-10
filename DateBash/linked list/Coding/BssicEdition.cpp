/*利用顺序表完成一个班级学生课程成绩的简单管理*/
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#define MAXSIZE 100 // 根据需要自己设定一个班级能够容纳的最大学生数
typedef struct stu
{
    int num;       // 学生学号
    char name[10]; // 学生姓名
    float score;   // 学生成绩
} STUDENT;         // 存放单个学生信息的结构体类型

typedef struct list
{
    STUDENT stu[MAXSIZE]; // 存放学生的数组定义，静态分配空间
    int length;           // 记录班级实际学生个数
} LIST;                   // 存放班级学生信息的顺序表类型

void listcreate(LIST *Li, int m)
// 初始化班级的学生信息
// m为该班级的初始人数
{
    int i;
    Li->length = 0;
    for (i = 1; i <= m; i++) // 输入m个学生的所有信息
    {
        printf("请输入第%d个学生的信息:\n", i);
        printf("学号=");
        scanf("%d", &Li->stu[i].num); // 输入第i个学生的学号
        printf("姓名=");
        scanf("%s", &Li->stu[i].name); // 输入第i个学生的姓名
        printf("成绩=");
        scanf("%f", &Li->stu[i].score); // 输入第i个学生的成绩
        Li->length++;                   // 学生人数加1
    }
}

int listinsert(LIST *Li, int i)
// 将学生插入到班级Li的第i个位置。
// 插入一个学生信息
{
    int j;
    STUDENT e;

    if (Li->length >= MAXSIZE - 1)
    // 测试存储空间是否被占满
    {
        printf("无更多的存储空间!\n");
        return 0;
    }

    if (i <= 0 || i >= Li->length + 2)
    // 插入位置检验，如果错误就返回0退出程序。
    {
        printf("插入位置有误!\n");
        return 0;
    }
    else
    {

        printf("请输入插入的学生信息:\n");
        printf("学号=");
        scanf("%d", &e.num);
        printf("姓名=");
        scanf("%s", e.name);
        printf("成绩=");
        scanf("%f", &e.score);
        // 利用for循环将第i个及其后面的元素依次往后移动
        for (j = Li->length; j >= i; j--)
        {
            Li->stu[j + 1] = Li->stu[j];
        }

        // 移开位置后将学生e放入到i位置
        Li->stu[i] = e;

        // 完成插入后，学生实际人数加1
        Li->length++;
        return 1;
    }
}

int listdel(LIST *Li, int i)
{
    int j;
    if (i <= 0 || i > Li->length) // 删除位置检验，如果错误就返回0退出程序。
        return 0;
    else
    {
        // 利用for循环将准备删除的第i个及其后面的元素依次往前移动
        for (j = i; j < Li->length; j++)
        {
            Li->stu[j] = Li->stu[j + 1];
        }

        // 删除第i个学生后，学生人数减1
        Li->length--;

        return 1;
    }
}

void listdisplay(LIST L)
{ // 显示所有学生信息
    int i;
    // printf("班级学生信息如下:\n");
    printf("      学号      姓名      成绩\n");
    for (i = 1; i <= L.length; i++)
        printf("%10d%10s%10.2f\n", L.stu[i].num, L.stu[i].name, L.stu[i].score);
}

void showmenu()
{ // 显示菜单
    printf("    欢迎使用成绩管理小软件\n");
    printf("\t1、创建学生信息\n");
    printf("\t2、插入学生信息\n");
    printf("\t3、删除学生信息\n");
    printf("\t4、显示学生信息\n");
    printf("\t5、退出程序\n");
}

int main()
{
    int no, stu_count, pos;
    LIST stu_info;
    while (1)
    {
        showmenu();
        printf("    请输入你的选择：");
        scanf("%d", &no);
        switch (no)
        {
        case 1:
            printf("班级信息初始化,按任意键继续……\n");
            getch();
            printf("请输入班级学生原始人数：");
            scanf("%d", &stu_count);
            listcreate(&stu_info, stu_count);
            system("cls");
            showmenu();
            listdisplay(stu_info);
            printf("班级信息初始化已经完成，按任意键继续……\n");
            getch();
            system("cls");
            break;
        case 2:
            printf("插入前班级信息：\n");
            listdisplay(stu_info);
            printf("请输入插入位置：");
            scanf("%d", &pos);
            listinsert(&stu_info, pos);
            printf("插入后班级信息：\n");
            listdisplay(stu_info);
            printf("插入已经完成，按任意键继续……\n");
            getch();
            system("cls");
            break;
        case 3:
            printf("删除前班级信息：\n");
            listdisplay(stu_info);
            printf("请输入删除位置：");
            scanf("%d", &pos);
            listdel(&stu_info, pos);
            printf("删除后班级信息：\n");
            listdisplay(stu_info);
            printf("删除已经完成，按任意键继续……\n");
            getch();
            system("cls");
            break;
        case 4:
            listdisplay(stu_info);
            printf("显示结果如上所示，按任意键继续……\n");
            getch();
            system("cls");
            break;
        default:
            printf("无效选择，请重新输入。\n");
        }
    }
    return 0;
}
