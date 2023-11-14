/*
 * @Description:
 * @Version: 2.0
 * @Autor: kerwinz
 * @Date: 2023-10-22 12:47:09
 * @LastEditors: kerwinz
 * @LastEditTime: 2023-10-22 13:00:38
 */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#define MAXSIZE 100 // 根据需要自己定义MAXSIZE为顺序栈的最大存储容量
typedef struct stack
{
    int data[MAXSIZE];
    int top;
} SEQSTACK;

void initstack(SEQSTACK *s) // 顺序栈初始化
{
    // 将栈顶指针指向初始的位置
    s->top = -1;
}

int empty(SEQSTACK *s) // 判断栈空
{
    if (s->top == -1)
        return 1;
    else
        return 0;
}

void push(SEQSTACK *s, int x) // 元素x进栈
{
    if (s->top == MAXSIZE - 1)
        printf("存储空间已满，元素进栈失败！\n");
    else
    {
        s->data[++s->top] = x; // 栈顶指针加1，将元素x压入栈顶指针所指向的位置
    }
}

int pop(SEQSTACK *s) // 元素出栈，出栈元素用e返回
{
    int e;
    if (empty(s) == -1)
    {
        printf("栈中元素已空，出栈元素失败！\n");
        return -99;
    }
    else
    {
        e = s->data[s->top--]; // 将栈顶指针所指向的元素赋值给e，栈顶指针减1
        return e;
    }
}

void conversion(SEQSTACK *s, int N, int r)
{ // 将十进制数N转换为r进制的数
    int x;
    initstack(s);
    while (N != 0) // 此循环为入栈操作
    {

        push(s, N % r); // 将N除以r所得的余数压入栈

        N = N / r; // N整除r所得的商赋值给N
    }
    while (!empty(s)) // 此循环为出栈操作
    {
        x = pop(s);
        if (x == 10)
            printf("A");
        else if (x == 11)
            printf("B");
        else if (x == 12)
            printf("C");
        else if (x == 13)
            printf("D");
        else if (x == 14)
            printf("E");
        else if (x == 15)
            printf("F");
        else
            printf("%d", x);
    }
    printf("\n");
}

int main()
{
    int number, r; // number为待准备转换的十进制数，r为进制
    SEQSTACK stack;
    char choice;
    while (1)
    {
        printf("请输入一个十进制整数:");

        scanf("%d", &number);

        printf("选择将该数转换为几进制数(2,8,16):");

        scanf("%d", &r);

        fflush(stdin);

        printf("转换后的结果为:");

        conversion(&stack, number, r);

        printf("是否继续?按N结束,其他任意键继续...");

        scanf("%c", &choice);

        system("cls");

        if (choice == 'N' || choice == 'n')

            break;
    }
    return 0;
}
