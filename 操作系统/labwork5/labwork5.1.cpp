#include <stdio.h>
#include <stdlib.h>
typedef struct LNode
{
    int address; // 表示该分区的起始地址
    int length;  // 表示该分区的长度
    int state;   // 0表示该分区空闲，1表示该分区已分配
    int number;  // 作业的编号
    struct LNode *next;
} *pLNode;

void Merge_free(pLNode L);
void InsertLNode(pLNode L, int addr, int len, int sta, int num)
{
    pLNode p = (pLNode)malloc(sizeof(LNode));
    p->address = addr;
    p->length = len;
    p->state = sta;
    p->number = num;
    p->next = NULL;
    L->next = p;
}
void InitLNode(pLNode L, int leng)
{

    // L = (pLNode)malloc(sizeof(LNode));
    L->address = 0; // 从首地址开始
    L->length = 8;
    L->state = 1;
    L->number = 65535;
    // 插入剩余的空闲分区
    InsertLNode(L, 8, leng - 8, 0, 0);
    printf("初始化成功！\n");
}
void Revocation(pLNode L, int num)
{
    pLNode p = L->next;
    pLNode prev = L;
    while (p != NULL)
    {
        if (p->number == num)
        {
            p->state = 0;
            printf("\033[1m\033[32mSuccessfully revoked job %d!\033[0m\n", num);
            Merge_free(L);
            return;
        }
        prev = p;
        p = p->next;
    }
    printf("\033[1m\033[31mRevocation failed, job %d not found.\033[0m\n", num);
}

void Merge_free(pLNode L)
{
    pLNode p = L->next;
    pLNode prev = L;
    while (p != NULL)
    {
        if (p->state == 0)
        {
            pLNode next = p->next;
            if (next != NULL && next->state == 0)
            {
                prev->next = next;
                p->length += next->length;
                p->next = next->next;
                free(next);
                p = prev->next;
            }
            else
            {
                prev = p;
                p = p->next;
            }
        }
        else
        {
            prev = p;
            p = p->next;
        }
    }
}
void Allocation(pLNode L, int len, int num, int algorithm)
{
    pLNode p = L->next;
    pLNode prev = L;
    while (p != NULL)
    {
        if (p->state == 0)
        {
            // 根据不同算法选择分配策略
            if (algorithm == 1)
            { // 最先适应算法
                if (p->length >= len)
                {
                    break;
                }
            }
            else if (algorithm == 2)
            { // 最佳适应算法
                if (p->length >= len && (p->length < prev->length || prev == L))
                {
                    break;
                }
            }
            else
            { // 最差适应算法
                if (p->length >= len && (prev->length < p->length || prev == L))
                {
                    break;
                }
            }
        }
        prev = p;
        p = p->next;
    }

    if (p != NULL)
    {
        if (p->length > len)
        {
            pLNode l = (pLNode)malloc(sizeof(LNode));
            l->length = p->length - len;
            l->address = p->address + len;
            l->state = 0;
            l->number = 0;
            l->next = p->next;
            p->next = l;
            p->length = len;
            p->number = num;
            p->state = 1;
            printf("\033[1m\033[32mMemory allocation successful!\033[0m\n");
        }
        else if (p->length == len)
        {
            p->number = num;
            p->state = 1;
            printf("\033[1m\033[32mMemory allocation successful!\033[0m\n");
        }
    }
    else
    {
        printf("\033[1m\033[31mMemory allocation failed, no suitable free block found.\033[0m\n");
    }
}

// 打印
void print(pLNode L)
{
    printf("\033[1m\033[34m"); // 设置字体为蓝色加粗
    printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
    printf("┃ 起始地址 ┃ 内存块长度 ┃ 存放作业编号 ┃ 内存块状态 ┃\n");
    printf("┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n");
    pLNode p = L;
    while (p != NULL)
    {
        printf("┃ %8d ┃ %10d ┃", p->address, p->length);
        if (p->number == 65535)
        {
            printf("    操作系统 ┃");
        }
        else if (p->number == 0)
        {
            printf("      无作业 ┃");
        }
        else
        {
            printf("%12d ┃", p->number);
        }
        if (p->state == 0)
        {
            printf("      空闲 ┃\n");
        }
        else
        {
            printf("    已分配 ┃\n");
        }
        p = p->next;
    }
    printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
    printf("\033[0m"); // 重置字体样式
}

// 服务选择菜单
int select()
{
    int a;
    printf("************************************************************\n");
    printf("请选择服务：\n");
    printf("1.为新作业分配内存\t2.撤销作业\n3.查看内存分配情况\t4.退出\n");
    printf("请输入编号以选择：");
    scanf("%d", &a);
    return a;
    printf("************************************************************\n");
}

int main() {
    pLNode L = (pLNode)malloc(sizeof(LNode));
    InitLNode(L, 5120);
    int a;
    int len, num, algorithm;
    
    while (1) {
        a = select();
        if (a == 4) {
            break;
        }
        switch (a) {
        case 1:
            printf("请选择分配算法 (1.最先适应, 2.最佳适应, 3.最差适应): ");
            scanf("%d", &algorithm);
            getchar(); // 消耗掉回车字符
            printf("请输入要分配内存的作业的长度和编号：");
            scanf("%d%d", &len, &num);
            getchar(); // 消耗掉回车字符
            Allocation(L, len, num, algorithm);
            break;
        case 2:
            printf("请输入要撤销作业的编号：");
            scanf("%d", &num);
            getchar(); // 消耗掉回车字符
            Revocation(L, num);
            break;
        case 3:
            // system("cls");
            print(L);
            break;
        default:
            printf("无效的选择,请重新输入。\n");
            getchar(); // 消耗掉回车字符
            break;
        }
    }
    
    return 0;
}