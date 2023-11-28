/*
 * @Description: 
 * @Version: 2.0
 * @Autor: kerwinz
 * @Date: 2023-11-28 08:46:10
 * @LastEditors: kerwinz
 * @LastEditTime: 2023-11-28 12:54:13
 */
#include <iostream>
#include <cctype>

// 定义二叉树节点结构
struct TreeNode
{
    char data;
    TreeNode *left;
    TreeNode *right;

    // 构造函数
    TreeNode(char data) : data(data), left(nullptr), right(nullptr) {}
};

// 递归建立表达式二叉树
TreeNode *buildExpressionTree(const char *expression, int &index)
{
    if (expression[index] == '\0')
    {
        return nullptr;
    }

    // 当前字符是操作数或运算符
    char currentChar = expression[index++];
    TreeNode *node = new TreeNode(currentChar);

    // 如果是运算符，则递归建立左右子树
    if (isdigit(currentChar) || isalpha(currentChar)) // 假设操作数是数字或字母
    {
        node->left = buildExpressionTree(expression, index);
        node->right = buildExpressionTree(expression, index);
    }

    return node;
}

// 用数组方式存储二叉树
void treeToArray(TreeNode *root, char *array, int &index)
{
    if (root == nullptr)
    {
        return;
    }

    array[index++] = root->data;
    treeToArray(root->left, array, index);
    treeToArray(root->right, array, index);
}

int main()
{
    const char *expression = "a+b*c";
    int index = 0;

    TreeNode *root = buildExpressionTree(expression, index);

    // 计算数组大小
    int arraySize = 0;
    treeToArray(root, nullptr, arraySize);

    // 创建数组
    char *expressionArray = new char[arraySize + 1];

    // 将二叉树存入数组
    index = 0;
    treeToArray(root, expressionArray, index);
    expressionArray[arraySize] = '\0'; // 添加字符串结束符

    // 输出数组内容
    std::cout << "Expression Tree Array: " << expressionArray << std::endl;

    // 释放内存
    delete[] expressionArray;
    // 释放二叉树的内存
    // 注意: 在实际应用中可能需要实现析构函数以递归释放所有节点
    delete root;

    return 0;
}
