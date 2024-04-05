#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <iostream>
using namespace std;

#define MAXSIZE 100

typedef char elemtype;

typedef struct bitree
{
    elemtype data;
    struct bitree *lchild, *rchild;
} BTREE;

BTREE *create()
{
    char ch;
    BTREE *bt;
    ch = getchar();
    if (ch == '#')
        bt = NULL;
    else
    {
        bt = (BTREE *)malloc(sizeof(BTREE));
        bt->data = ch;
        bt->lchild = create();
        bt->rchild = create();
    }
    return (bt);
}

void preorder(BTREE *root)
{
    BTREE *p;
    p = root;
    if (p != NULL)
    {
        cout << p->data << " ";
        preorder(p->lchild);
        preorder(p->rchild);
    }
}

void inorder(BTREE *root)
{
    BTREE *p;
    p = root;
    if (p != NULL)
    {
        inorder(p->lchild);
        cout << p->data << " ";
        inorder(p->rchild);
    }
}

void postorder(BTREE *root)
{
    BTREE *p;
    p = root;
    if (p != NULL)
    {
        postorder(p->lchild);
        postorder(p->rchild);
        cout << p->data << " ";
    }
}

void lorder(BTREE *root)
{
    BTREE *q[MAXSIZE], *p;
    int f, r;
    q[1] = root;
    f = r = 1;
    while (f <= r)
    {
        p = q[f];
        f++;
        cout << p->data << " ";
        if (p->lchild != NULL)
        {
            r++;
            q[r] = p->lchild;
        }
        if (p->rchild != NULL)
        {
            r++;
            q[r] = p->rchild;
        }
    }
}

void showmenu()
{
    cout << "    欢迎使用二叉树操作演示软件" << endl;
    cout << "\t1、创建二叉树" << endl;
    cout << "\t2、先序遍历二叉树" << endl;
    cout << "\t3、中序遍历二叉树" << endl;
    cout << "\t4、后序遍历二叉树" << endl;
    cout << "\t5、层次遍历二叉树" << endl;
    cout << "\t6、退出程序" << endl;
}

int main()
{
    BTREE *root = NULL;
    int no;
    while (1)
    {
        showmenu();
        cout << "   请输入你的选择:";
        cin >> no;
        cin.ignore();
        switch (no)
        {
        case 1:
            cout << "请按先序依次输入二叉树的结点,\n";
            cout << "空结点用#号表示.\n";
            root = create();
            cout << "二叉树创建成功,按任意键继续...\n";
            cin.get();
            system("cls");
            break;
        case 2:
            cout << "二叉树先序遍历结果为:\n";
            preorder(root);
            cout << "\n";
            system("pause");
            system("cls");
            break;
        case 3:
            cout << "二叉树中序遍历结果为:\n";
            inorder(root);
            cout << "\n";
            system("pause");
            system("cls");
            break;
        case 4:
            cout << "二叉树后序遍历结果为:\n";
            postorder(root);
            cout << "\n";
            system("pause");
            system("cls");
            break;
        case 5:
            cout << "二叉树层次遍历结果为:\n";
            lorder(root);
            cout << "\n";
            system("pause");
            system("cls");
            break;
        case 6:
            return 0;
        default:
            cout << "你的输入有误,请重新输入!\n";
            system("pause");
            system("cls");
        }
    }
    return 0;
}