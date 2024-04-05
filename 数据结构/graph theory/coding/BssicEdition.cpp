#include <stdio.h>
#include <stdlib.h>
#define vertexnum 100 // 定义最大可输入的结点个数
typedef struct node   // 定义图形的顶点结构
{
    int vertex; // 图中的顶点信息为数字
    struct node *next;
} Graph;
Graph head[vertexnum];  // 邻接表的表头结点
int Visited[vertexnum]; // 遍历记录

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
    printf("\t3、图的深度优先遍历\n");
    printf("\t4、退出程序\n");
}

void print_l_graph(Graph *head)
{ // 输出邻接链表的数据
    Graph *searchP;
    searchP = head->next;
    while (searchP != NULL)
    {
        printf("%d ", searchP->vertex);
        searchP = searchP->next;
    }
    printf("\n");
}

void DFS(int vertex)
{                        // 深度优先遍历
    Graph *SearchP;      // 结点声明
    Visited[vertex] = 1; // 标记某个结点已遍历过
    printf("[%d]==>", vertex);
    SearchP = head[vertex].next;
    while (SearchP != NULL)
    {
        if (Visited[SearchP->vertex] == 0) // 判断结点未被遍历过
            DFS(SearchP->vertex);          // 递归调用深度优先遍历函数
        SearchP = SearchP->next;           // 下一个邻接点
    }
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
            for (i = 1; i <= vermax; i++)
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
            printf("图的深度优先遍历结果为：\n");
            DFS(source);
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
}