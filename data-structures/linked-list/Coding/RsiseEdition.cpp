/*利用单链表完成一个班级学生课程成绩的简单管理*/ 
#define NULL 0
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
typedef struct stu
{
    int num;       // 学生的学号
    char name[10]; // 学生的姓名
    float score;   // 学生的成绩
} STUDENT;         // 存放单个学生信息的结构体类型

typedef struct node
{
    STUDENT data;      // 结点的值
    struct node *next; // 指向下一个结点的地址
} SNODE;

void showmenu()
{ // 显示菜单
    printf("    欢迎使用成绩管理小软件\n");
    printf("\t1、创建学生信息\n");
    printf("\t2、插入学生信息\n");
    printf("\t3、删除学生信息\n");
    printf("\t4、显示学生信息\n");
    printf("\t5、退出程序\n");
}

SNODE *listcreate(SNODE *head, int n) // n为该班级的实际人数
{                                     // 建立班级学生信息
    int i;
    SNODE *p = NULL, *q = NULL;
    p = head;
    for (i = 1; i <= n; i++) // 循环插入n个学生
    {
        printf("\n请输入第%d位学生的信息:\n", i);
        q = (SNODE *)malloc(sizeof(SNODE));
        printf("学号=");
        scanf("%d", &q->data.num);
        printf("姓名=");
        scanf("%s", q->data.name);
        printf("成绩=");
        scanf("%f", &q->data.score);
        q->next = NULL;
        p->next = q;
        p = q;
        head->data.num++;
    }
    return head;
}

SNODE *listinsert(SNODE *head, int i)
{
    SNODE *p, *q;
    int j = 0;
    p = head;
    while (p != NULL && j < i - 1)
    {
        p = p->next;
        j++;
    }
    if (p == NULL || i < 1)
    {
        printf("插入位置有误!\n");
        return head;
    }
    else
    {
        q = (SNODE *)malloc(sizeof(SNODE));
        printf("请输入插入的学生信息:\n");
        printf("学号=");
        scanf("%d", &q->data.num);
        printf("姓名=");
        scanf("%s", q->data.name);
        printf("成绩=");
        scanf("%f", &q->data.score);
        q->next = p->next;
        p->next = q;
        head->data.num++;
        return head;
    }
}

SNODE *listdel(SNODE *head, int i)
{
    SNODE *p, *q;
    int j = 0;
    p = head;
    while (p->next != NULL && j < i - 1)
    {
        p = p->next;
        j++;
    }
    if (p->next == NULL || i < 1)
    {
        printf("删除位置有误!\n");
        return head;
    }
    else
    {
        q = p->next;
        p->next = q->next;
        free(q);
        head->data.num--;
        return head;
    }
}
void listdisplay(SNODE *head)
{
    // 显示所有学生信息
    SNODE *p;
    p = head->next;
    printf("班级学生信息如下:\n");
    printf("      学号      姓名      成绩\n");
    while (p != NULL)
    {
        printf("%10d%10s%10.2f\n", p->data.num, p->data.name, p->data.score);
        p = p->next;
    }
}

int main()
{
    SNODE *head = NULL;
    int no, stu_count, pos;
    head = (SNODE *)malloc(sizeof(SNODE)); // 动态建立第一个结点，作为头结点，head指针指向它，
    head->data.num = 0;                    // 链表为带头结点的单链表
    head->next = NULL;
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
            head = listcreate(head, stu_count);
            system("cls");
            showmenu();
            listdisplay(head);
            printf("班级信息初始化已经完成，按任意键继续……\n");
            getch();
            system("cls");
            break;
        case 2:
            printf("插入前班级信息：\n");
            listdisplay(head);
            printf("请输入插入位置：");
            scanf("%d", &pos);
            head = listinsert(head, pos);
            printf("插入后班级信息：\n");
            listdisplay(head);
            printf("插入已经完成，按任意键继续……\n");
            getch();
            system("cls");
            break;
        case 3:
            printf("删除前班级信息：\n");
            listdisplay(head);
            printf("请输入删除位置：");
            scanf("%d", &pos);
            head = listdel(head, pos);
            printf("删除后班级信息：\n");
            listdisplay(head);
            printf("删除已经完成，按任意键继续……\n");
            getch();
            system("cls");
            break;
        case 4:
            listdisplay(head);
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
