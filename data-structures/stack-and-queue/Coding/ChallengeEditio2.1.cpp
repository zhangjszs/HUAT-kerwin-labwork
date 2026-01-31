/*
 * @Description:
 * @Version: 2.0
 * @Autor: kerwinz
 * @Date: 2023-10-23 20:49:40
 * @LastEditors: kerwinz
 * @LastEditTime: 2023-10-23 22:20:52
 */
/*
将任意一个十进制小数转换成R进制数，任意输入一个小数后，自动转换得到相应进制的数。
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LEN 100

typedef struct
{
    int data[MAX_LEN];
    int top;
} Stack;

typedef struct
{
    int data[MAX_LEN];
    int front, rear;
} Queue;

void initStack(Stack *s)
{
    s->top = -1;
}

void push(Stack *s, int x)
{
    s->data[++s->top] = x;
}

int pop(Stack *s)
{
    return s->data[s->top--];
}

void initQueue(Queue *q)
{
    q->front = q->rear = 0;
}

void enqueue(Queue *q, int x)
{
    q->data[q->rear++] = x;
}

int dequeue(Queue *q)
{
    return q->data[q->front++];
}

int isEmptys(Stack s)
{
    return s.top == -1;
}

int isEmptyq(Queue q)
{
    return q.front == q.rear;
}

void decimalToAny(double num, int r)
{
    Stack s;
    Queue q;
    initStack(&s);
    initQueue(&q);

    int intPart = (int)num;
    // printf("%d\n", intPart);
    while (intPart != 0)
    {
        push(&s, intPart % r);
        intPart /= r;
    }

    double fracPart = num - (int)num;

    while (fracPart > 0.0000001)
    {
        fracPart *= r;
        int digit = (int)fracPart;
        enqueue(&q, digit);
        fracPart -= digit;
    }

    while (!isEmptys(s))
    {
        printf("%d", pop(&s));
    }

    if (!isEmptyq(q))
    {
        printf(".");
        while (!isEmptyq(q))
        {
            printf("%d", dequeue(&q));
        }
    }
}

int main()
{
    double num;
    int r;

    printf("Enter a decimal number: ");
    scanf("%lf", &num);

    printf("Enter the base: ");
    scanf("%d", &r);

    decimalToAny(num, r);

    return 0;
}