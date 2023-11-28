#include <iostream>
#include <stack>
using namespace std;

// 表达式二叉树的节点
struct Node {
    char data;
    Node* left;
    Node* right;
    Node(char d) : data(d), left(nullptr), right(nullptr) {}
};

// 递归构建表达式二叉树
Node* buildExpressionTree(string expression) {
    stack<Node*> s;
    for (char ch : expression) {
        Node* newNode = new Node(ch);
        if (isdigit(ch)) {
            s.push(newNode);
        } else {
            newNode->right = s.top();
            s.pop();
            newNode->left = s.top();
            s.pop();
            s.push(newNode);
        }
    }
    return s.top();
}

// 前序遍历
void preorderTraversal(Node* root) {
    if (root == nullptr) {
        return;
    }
    cout << root->data << " ";
    preorderTraversal(root->left);
    preorderTraversal(root->right);
}

// 中序遍历
void inorderTraversal(Node* root) {
    if (root == nullptr) {
        return;
    }
    inorderTraversal(root->left);
    cout << root->data << " ";
    inorderTraversal(root->right);
}

// 后序遍历
void postorderTraversal(Node* root) {
    if (root == nullptr) {
        return;
    }
    postorderTraversal(root->left);
    postorderTraversal(root->right);
    cout << root->data << " ";
}

// 计算表达式的结果
int evaluateExpression(Node* root) {
    if (root->left == nullptr && root->right == nullptr) {
        return root->data - '0';
    }
    int leftValue = evaluateExpression(root->left);
    int rightValue = evaluateExpression(root->right);
    switch (root->data) {
        case '+':
            return leftValue + rightValue;
        case '-':
            return leftValue - rightValue;
        case '*':
            return leftValue * rightValue;
        case '/':
            return leftValue / rightValue;
        default:
            return 0;
    }
}

int main() {
    string expression;
    cout << "请输入表达式：";
    cin >> expression;

    Node* root = buildExpressionTree(expression);

    cout << "前序遍历结果：";
    preorderTraversal(root);
    cout << endl;

    cout << "中序遍历结果：";
    inorderTraversal(root);
    cout << endl;

    cout << "后序遍历结果：";
    postorderTraversal(root);
    cout << endl;

    cout << "表达式结果：" << evaluateExpression(root) << endl;

    return 0;
}