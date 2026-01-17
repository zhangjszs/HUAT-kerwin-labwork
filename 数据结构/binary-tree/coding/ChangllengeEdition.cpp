/*
 * @Description:
 * @Version: 2.0
 * @Autor: kerwinz
 * @Date: 2023-11-28 08:46:10
 * @LastEditors: kerwinz
 * @LastEditTime: 2023-12-04 10:42:59
 */
#include <iostream>
#include <stack>
#include <cctype>
using namespace std;

// 二叉树节点
struct Node
{
    char data;
    Node *left;
    Node *right;
    Node(char x) : data(x), left(NULL), right(NULL) {}
};

// 创建二叉树
Node *createTree(string exp)
{
    stack<Node *> s;
    for (char x : exp)
    {
        if (x == '+' || x == '-' || x == '*' || x == '/')
        {
            Node *r = s.top();
            s.pop();
            Node *l = s.top();
            s.pop();
            Node *n = new Node(x);
            n->left = l;
            n->right = r;
            s.push(n);
        }
        else
        {
            Node *n = new Node(x);
            s.push(n);
        }
    }
    return s.top();
}

// 前序遍历
void preorder(Node *root)
{
    if (root)
    {
        cout << root->data << " ";
        preorder(root->left);
        preorder(root->right);
    }
}

// 中序遍历
void inorder(Node *root)
{
    if (root)
    {
        inorder(root->left);
        cout << root->data << " ";
        inorder(root->right);
    }
}

// 后序遍历
void postorder(Node *root)
{
    if (root)
    {
        postorder(root->left);
        postorder(root->right);
        cout << root->data << " ";
    }
}

int precedence(char c)
{
    if (c == '+' || c == '-')
        return 1;
    if (c == '*' || c == '/')
        return 2;
    return 0;
}

// 计算表达式的值
string infixToPostfix(string exp)
{
    stack<char> s;
    string postfix;
    for (char c : exp)
    {
        if (isdigit(c))
            postfix += c;
        else if (c == '(')
            s.push(c);
        else if (c == ')')
        {
            while (s.top() != '(')
            {
                postfix += s.top();
                s.pop();
            }
            s.pop();
        }
        else
        {
            while (!s.empty() && precedence(s.top()) >= precedence(c))
            {
                postfix += s.top();
                s.pop();
            }
            s.push(c);
        }
    }
    while (!s.empty())
    {
        postfix += s.top();
        s.pop();
    }
    return postfix;
}

int evaluatePostfix(string exp)
{
    stack<int> s;

    for (char c : exp)
    {
        if (isdigit(c))
        {
            s.push(c - '0');
        }
        else
        {
            int r = s.top();
            s.pop();
            int l = s.top();
            s.pop();

            switch (c)
            {
            case '+':
                s.push(l + r);
                break;
            case '-':
                s.push(l - r);
                break;
            case '*':
                s.push(l * r);
                break;
            case '/':
                s.push(l / r);
                break;
            }
        }
    }

    return s.top();
}

int main()
{

    string exp;
    cout << "请输入表达式:";
    cin >> exp;

    string postfix = infixToPostfix(exp);

    Node *root = createTree(postfix);
    cout << "前序遍历:";
    preorder(root);
    cout << "\n";
    cout << "中序遍历:";
    inorder(root);
    cout << "\n";
    cout << "后序遍历:";
    postorder(root);
    cout << "\n";
    cout << "表达式的值:" << evaluatePostfix(postfix) << "\n";
    return 0;
}