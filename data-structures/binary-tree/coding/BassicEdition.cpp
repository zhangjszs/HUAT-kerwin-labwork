/*
 * @Author: kerwin-win zhangjszs@foxmail.com
 * @Date: 2023-12-22 12:58:38
 * @LastEditors: kerwin-win zhangjszs@foxmail.com
 * @LastEditTime: 2024-03-15 11:59:29
 * @FilePath: \HUAT-kerwin-labwork\数据结构\BinaryTree\coding\BassicEdition.cpp
 * @Description: 二叉树基本操作演示程序
 *
 * 功能：创建二叉树并实现先序、中序、后序、层次遍历
 * 算法：使用队列进行层次创建，使用栈进行非递归遍历
 */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <assert.h>

#define MAX_SIZE 100

typedef char ElemType;
typedef struct BiTree {
    ElemType data;
    struct BiTree *lchild, *rchild;
} BiTree;

/**
 * @brief 非递归创建二叉树
 *
 * 使用层次遍历的方式创建二叉树，空节点用逗号表示，结束用#
 * 算法：维护一个队列，按层次顺序构建树结构
 *
 * @return BiTree* 创建的二叉树根节点
 */
BiTree *create() {
    BiTree *queue[MAX_SIZE];     // 队列存放二叉树节点
    BiTree *node;                // 当前节点
    BiTree *root = NULL;         // 根节点
    int front = 0, rear = -1;    // 队列头尾指针
    char ch;

    // 初始化队列
    for (int i = 0; i < MAX_SIZE; i++) {
        queue[i] = NULL;
    }

    printf("请按层次依次输入二叉树中的结点:\n");
    printf("空结点以逗号代替，以#号结束!\n");

    ch = getchar();
    while (ch != '#') {
        node = NULL;
        if (ch != ',') {  // 非空节点
            node = (BiTree *)malloc(sizeof(BiTree));
            if (node == NULL) {
                fprintf(stderr, "内存分配失败!\n");
                exit(1);
            }
            node->data = ch;
            node->lchild = NULL;
            node->rchild = NULL;
        }

        // 入队
        if (++rear >= MAX_SIZE) {
            fprintf(stderr, "队列溢出!\n");
            exit(1);
        }
        queue[rear] = node;

        // 设置根节点
        if (rear == 0) {
            root = node;
        } else {
            // 连接父子关系
            if (node != NULL && queue[front] != NULL) {
                if ((rear - front) % 2 == 1) {
                    queue[front]->lchild = node;  // 左孩子
                } else {
                    queue[front]->rchild = node;  // 右孩子
                }
            }
            // 出队条件
            if ((rear - front) % 2 == 0) {
                front++;
            }
        }
        ch = getchar();
    }
    return root;
}

/**
 * @brief 非递归先序遍历二叉树
 *
 * 算法：使用栈模拟递归，先访问根节点，再遍历左子树，最后右子树
 * 时间复杂度：O(n)，空间复杂度：O(h)，h为树高
 *
 * @param root 二叉树根节点
 */
void preorder(BiTree *root) {
    if (root == NULL) return;

    BiTree *stack[MAX_SIZE];
    int top = -1;
    BiTree *p = root;

    while (p != NULL || top != -1) {
        while (p != NULL) {
            printf("%c", p->data);  // 访问根节点
            if (++top >= MAX_SIZE) {
                fprintf(stderr, "栈溢出!\n");
                exit(1);
            }
            stack[top] = p;
            p = p->lchild;  // 遍历左子树
        }
        if (top != -1) {
            p = stack[top--];
            p = p->rchild;  // 遍历右子树
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

int main() {
    BiTree *root = NULL;
    int choice;

    while (1) {
        showmenu();
        printf("请输入你的选择：");
        if (scanf("%d", &choice) != 1) {
            fprintf(stderr, "输入错误，请输入数字!\n");
            while (getchar() != '\n');  // 清空缓冲区
            continue;
        }
        fflush(stdin);

        switch (choice) {
        case 1:
            root = create();
            printf("二叉树创建成功，按任意键继续…\n");
            getch();
            system("cls");
            break;
        case 2:
            if (root == NULL) {
                printf("请先创建二叉树!\n");
            } else {
                printf("二叉树先序遍历结果为：\n");
                preorder(root);
                printf("\n");
            }
            system("pause");
            system("cls");
            break;
        // 类似修改其他case
        case 6:
            printf("退出程序\n");
            return 0;
        default:
            printf("你的输入有误，请重新输入！\n");
        }
    }
    return 0;
}
