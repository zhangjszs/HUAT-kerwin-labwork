#include <stdio.h>
#include <stdlib.h>
#define vertexnum 100 // 定义最大可输入的结点个数
#define QueueMax 100
typedef struct node // 定义图形的顶点结构
{
    int vertex; // 图中的顶点信息为数字
    struct node *next;
} Graph;

Graph head[vertexnum];  // 邻接表的表头结点
int Visited[vertexnum]; // 遍历记录
int Front = -1;
int Rear = -1;
int Queue[QueueMax];

int Enqueue(int Vertex) // 元素入队
{
    if (Rear >= QueueMax) // 队列已满
        return -1;
    else
    {
        Rear++;               // 队列尾端指针后移
        Queue[Rear] = Vertex; // 将值存入队列中
        return 1;
    }
}

int Dequeue() // 元素出队
{
    if (Front >= Rear) // 队列已空
        return -1;
    else
    {
        Front++; // 队头指针后移
        return Queue[Front];
    }
}

void BFS(int Vertex) // 广度优先搜索
{
    Enqueue(Vertex);     // 将起始顶点入队
    Visited[Vertex] = 1; // 标记起始顶点已访问

    while (Front < Rear) // 队列非空时循环
    {
        int current = Dequeue(); // 出队一个顶点
        printf("%d ", current);  // 访问该顶点

        Graph *searchP = head[current].next;
        while (searchP != NULL) // 遍历当前顶点的邻接顶点
        {
            int neighbor = searchP->vertex;
            if (Visited[neighbor] == 0) // 若邻接顶点未访问过
            {
                Enqueue(neighbor);     // 将邻接顶点入队
                Visited[neighbor] = 1; // 标记邻接顶点已访问
            }
            searchP = searchP->next;
        }
    }
}

void Create_l_Graph(int Vertex1, int Vertex2, int no)
{                   // 以邻接链表建立图形
    Graph *searchP; // 结点声明
    Graph *New;     // 新结点声明
    New = (Graph *)malloc(sizeof(struct node));
    if (New != NULL)
    {
        New->vertex = Vertex2;
        New->next = NULL;
        searchP = &(head[Vertex1]);
        while (searchP->next != NULL)
            searchP = searchP->next;
        searchP->next = New;
        if (no == 0)
        {
            New = (Graph *)malloc(sizeof(struct node));
            New->vertex = Vertex1;
            New->next = NULL;
            searchP = &(head[Vertex2]);
            while (searchP->next != NULL)
                searchP = searchP->next;
            searchP->next = New;
        }
    }
}

void showmenu()
{ // 显示菜单
    printf("    欢迎使用图的操作演示软件\n");
    printf("\t1、创建图的邻接表\n");
    printf("\t2、图的输出\n");
    printf("\t3、图的广度优先遍历\n");
    printf("\t4、退出程序\n");
}

void print_l_graph(Graph *head)
{ // 输出邻接链表的数据
    Graph *searchP;
    searchP = head->next;
    while (searchP != NULL)
    {
        printf("[%d]", searchP->vertex);
        searchP = searchP->next;
    }
    printf("\n");
}

int main()
{
    int source;      // 图中一条边的起始顶点
    int destination; // 图中一条边的终止顶点
    int i, j;
    int vermax;  // 定义图中最大的顶点数
    int edgemax; // 定义图中最大的边数
    int choice;
    int no;

    while (1)
    {
        showmenu();
        printf("   请输入你的选择：");
        scanf("%d", &choice);
        fflush(stdin); // 清除键盘缓冲区
        switch (choice)
        {
        case 1:
            printf("请输入图的类别（有向图-1，无向图-0）:");
            scanf("%d", &no);
            printf("请输入图中的总顶点数和边数:");
            scanf("%d%d", &vermax, &edgemax);
            for (i = 1; i < vermax; i++)
            {
                head[i].vertex = i;
                head[i].next = NULL;
            }
            for (i = 1; i <= edgemax; i++)
            {
                printf("请输入第%d条边的起点：", i);
                scanf("%d", &source);
                printf("请输入第%d条边的终点：", i);
                scanf("%d", &destination);
                if (source == destination)
                    printf("输入有误！\n"); // 出错：自身循环
                else                        // 调用建立邻接链表
                    Create_l_Graph(source, destination, no);
            }
            printf("图创建成功，按任意键继续…\n");
            getchar();
            system("cls");
            break;
        case 2:
            printf("图的邻接表如下：\n");
            for (i = 1; i <= vermax; i++)
            {
                printf("顶点[%d]:", i);
                print_l_graph(&head[i]); // 调用输出邻接链表数据
            }
            printf("\n");
            system("pause");
            system("cls");
            break;
        case 3:
            for (i = 1; i <= vermax; i++)
                Visited[i] = 0;
            printf("请输入遍历的起点:");
            scanf("%d", &source);
            printf("图的广度优先遍历结果为：\n");
            BFS(source);
            printf("END\n");
            system("pause");
            system("cls");
            break;
        case 4:
            return 0;
        default:
            printf("你的输入有误，请重新输入！\n");
            system("pause");
            system("cls");
        }
    }

    return 0;
}