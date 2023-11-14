// 链队列基本操作
// 根据实际处理数据的类型定义链队中结点的值域类型elemtype
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
typedef int elemtype;
typedef struct node // 队列结点类型定义
{
	elemtype data;	   // 队列的数据元素类型
	struct node *next; // 指向后继结点的指针
} NODE;
typedef struct
{						// 定义链队
	NODE *front, *rear; // 定义链队队头和队尾指针
} LINKQUEUE;

void initqueue(LINKQUEUE *QL) // 队列的初始化
{
	QL->front = (NODE *)malloc(sizeof(NODE)); // 队列为带头结点的链队列
	QL->front->next = NULL;
	QL->rear = QL->front;
}
LINKQUEUE *pushqueue(LINKQUEUE *QL, elemtype x)
{
	// 将元素x插入到链队列QL中，作为QL的新队尾
	NODE *p;
	p = (NODE *)malloc(sizeof(NODE));
	p->data = x;
	p->next = NULL;
	QL->rear->next = p;
	QL->rear = p;
	return QL;
}
elemtype popqueue(LINKQUEUE *QL)
{ // 若链队列不为空，则删除队头元素，返回其元素值
	NODE *p;
	elemtype x;
	if (QL->front == QL->rear)
	{
		printf("队列空!");
		return 0;
	}
	p = QL->front->next;
	x = p->data;
	QL->front->next = p->next;
	if (QL->rear == p)
		QL->rear = QL->front;
	free(p);
	return x;
}

void printqueue(LINKQUEUE *QL) // 队列的显示
{
	NODE *p;
	p = QL->front->next;
	if (p == NULL)
		printf("队列空!");
	while (p != NULL)
	{
		if (p->next == NULL)
			printf("%d", p->data);
		else
			printf("%d<--", p->data);
		p = p->next;
	}
	printf("\n");
}
int main()
{
	LINKQUEUE *p;
	int choice, elemdata, x = 0;
	p = (LINKQUEUE *)malloc(sizeof(LINKQUEUE));
	initqueue(p);
	while (1)
	{
		printf("      欢迎使用队列操作小程序：\n");
		printf("\t1、元素入队\n");
		printf("\t2、元素出队\n");
		printf("\t3、显示队列\n");
		printf("\t4、清屏幕\n");
		printf("\t5、退出程序\n");
		printf("      请选择你的操作：");
		scanf("%d", &choice);
		switch (choice)
		{
		case 1:
			printf("请输入进队元素：");
			scanf("%d", &elemdata);
			p = pushqueue(p, elemdata);
			printf("队列中的元素为：\n");
			printqueue(p);
			system("pause");
			break;
		case 2:
			x = popqueue(p);
			if (x != 0)
				printf("元素%d出队!\n", x);
			printf("队列中的元素为：\n");
			printqueue(p);
			system("pause");
			break;
		case 3:
			printf("队列中的元素分别为：\n");
			printqueue(p);
			system("pause");
			break;
		case 4:
			system("cls");
			break;
		default:
			printf("错误选项 程序截至运行\n");
			exit(0);
		}
		system("cls");
	}
	return 0;
}
