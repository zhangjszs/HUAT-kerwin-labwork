#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#define MAXSIZE 100

typedef char elemtype;
typedef struct bitree
{
    elemtype data;
    struct bitree *lchild, *rchild;
} BTREE;

BTREE *create()
{                               // 非递归创建二叉树
    BTREE *q[100];              // 定义q数组作为队列存放二叉链表中结点，100为最大容量
    BTREE *s;                   // 二叉链表中的结点
    BTREE *root;                // 二叉链表的根指针
    int front = 1, rear = 0, i; // 定义队列的头、尾指针
    char ch;                    // 结点的data域值
    root = NULL;
    for (i = 0; i < 100; i++)
        q[i] = NULL;
    printf("请按层次依次输入二叉树中的结点:\n");
    printf("空结点以逗号代替，以#号结束!\n");
    ch = getchar();
    while (ch != '#') // 输入值为#号,算法结束
    {
        s = NULL;
        if (ch != ',') // 输入数据不为逗号,表示不为虚结点,否则为虚结点
        {
            s = (BTREE *)malloc(sizeof(BTREE));
            s->data = ch;
            s->lchild = NULL;
            s->rchild = NULL;
        }
        rear++;
        q[rear] = s; // 新结点或虚结点进队
        if (rear == 1)
            root = s;
        else
        {
            if ((s != NULL) && (q[front] != NULL))
            {
                if (rear % 2 == 0)
                    q[front]->lchild = s; // rear为偶数,s为双亲左孩子
                else
                    q[front]->rchild = s; // rear为奇数,s为双亲右孩子
            }
            if (rear % 2 == 1)
                front++; // 出队
        }
        ch = getchar();
    }
    return root;
}

void preorder(BTREE *root)
{                          // 非递归实现的先序遍历
    BTREE *stack[MAXSIZE]; // 栈存放结点
    int top = -1;          // 栈顶指针初始化为-1
    BTREE *p = root;       // 指针p指向根结点

    while (p != NULL || top != -1)
    {
        while (p != NULL)
        {                          // 一直向左走直到最左结点
            printf("%c", p->data); // 先序遍历，输出结点值
            stack[++top] = p;      // 结点入栈
            p = p->lchild;
        }
        if (top != -1)
        {
            p = stack[top--]; // 出栈并切换到右子树
            p = p->rchild;
        }
    }
}

void inorder(BTREE *root)
{                          // 非递归实现的中序遍历
    BTREE *stack[MAXSIZE]; // 栈存放结点
    int top = -1;          // 栈顶指针初始化为-1
    BTREE *p = root;       // 指针p指向根结点

    while (p != NULL || top != -1)
    {
        while (p != NULL)
        {                     // 一直向左走直到最左结点
            stack[++top] = p; // 结点入栈
            p = p->lchild;
        }
        if (top != -1)
        {
            p = stack[top--];      // 出栈并访问结点
            printf("%c", p->data); // 中序遍历，输出结点值
            p = p->rchild;         // 切换到右子树
        }
    }
}

void postorder(BTREE *root)
{                            // 非递归实现的后序遍历
    BTREE *p, *s1[100];      // s1栈存放树中结点
    int s2[100], top = 0, b; // s2栈存放进栈标志
    p = root;
    do
    {
        while (p != NULL)
        {
            s1[top] = p;
            s2[top++] = 0; // 第一次进栈标志为0
            p = p->lchild;
        }
        if (top > 0)
        {
            b = s2[--top];
            p = s1[top];
            if (b == 0)
            {
                s1[top] = p;
                s2[top++] = 1; // 第二次进栈标志为0
                p = p->rchild;
            }
            else
            {
                printf("%c", p->data);
                p = NULL;
            }
        }
    } while (top > 0);
}

void lorder(BTREE *root)
{                          // 非递归实现的层次遍历
    BTREE *q[MAXSIZE], *p; // maxsize为最大容量
    int f, r;              // f,r类似于头尾指针
    q[1] = root;
    f = r = 1;
    while (f <= r)
    {
        p = q[f];
        f++; // 出队
        printf("%c", p->data);
        if (p->lchild != NULL)
        {
            r++;
            q[r] = p->lchild;
        } // 入队
        if (p->rchild != NULL)
        {
            r++;
            q[r] = p->rchild;
        } // 入队
    }
}

void showmenu()
{ // 显示菜单
    printf("    欢迎使用二叉树操作演示小软件\n");
    printf("\t1、创建二叉树\n");
    printf("\t2、先序遍历二叉树\n");
    printf("\t3、中序遍历二叉树\n");
    printf("\t4、后序遍历二叉树\n");
    printf("\t5、层次遍历二叉树\n");
    printf("\t6、退出程序\n");
}

int main()
{
    BTREE *root = NULL;
    int no;
    while (1)
    {
        showmenu();
        printf("   请输入你的选择：");
        scanf("%d", &no);
        fflush(stdin); // 清除键盘缓冲区
        switch (no)
        {
        case 1:
            root = create();
            printf("二叉树创建成功，按任意键继续…\n");
            getch();
            system("cls");
            break;
        case 2:
            printf("二叉树先序遍历结果为：\n");
            preorder(root);
            printf("\n");
            system("pause");
            system("cls");
            break;
        case 3:
            printf("二叉树中序遍历结果为：\n");
            inorder(root);
            printf("\n");
            system("pause");
            system("cls");
            break;
        case 4:
            printf("二叉树后序遍历结果为：\n");
            postorder(root);
            printf("\n");
            system("pause");
            system("cls");
            break;
        case 5:
            printf("二叉树层次遍历结果为：\n");
            lorder(root);
            printf("\n");
            system("pause");
            system("cls");
            break;
        default:
            printf("你的输入有误，请从新输入！\n");
        }
    }
    return 0;
}
