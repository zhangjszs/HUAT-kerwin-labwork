#include <iostream>
#include <cassert>

/**
 * @brief 二叉搜索树节点
 */
struct TreeNode {
    int value;
    TreeNode* left;
    TreeNode* right;

    TreeNode(int val) : value(val), left(nullptr), right(nullptr) {}
};

/**
 * @brief 二叉搜索树类
 */
class BinarySearchTree {
private:
    TreeNode* m_root;

    TreeNode* insertHelper(TreeNode* node, int value) {
        if (node == nullptr) {
            return new TreeNode(value);
        }

        if (value < node->value) {
            node->left = insertHelper(node->left, value);
        } else if (value > node->value) {
            node->right = insertHelper(node->right, value);
        }

        return node;
    }

    TreeNode* searchHelper(TreeNode* node, int value) {
        if (node == nullptr || node->value == value) {
            return node;
        }

        if (value < node->value) {
            return searchHelper(node->left, value);
        } else {
            return searchHelper(node->right, value);
        }
    }

    void inorderHelper(TreeNode* node, int* result, int& index) {
        if (node != nullptr) {
            inorderHelper(node->left, result, index);
            result[index++] = node->value;
            inorderHelper(node->right, result, index);
        }
    }

public:
    BinarySearchTree() : m_root(nullptr) {}

    ~BinarySearchTree() {
        destroyTree(m_root);
    }

    void destroyTree(TreeNode* node) {
        if (node != nullptr) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

    void insert(int value) {
        m_root = insertHelper(m_root, value);
    }

    TreeNode* search(int value) {
        return searchHelper(m_root, value);
    }

    void inorderTraversal(int* result, int& size) {
        int index = 0;
        inorderHelper(m_root, result, index);
        size = index;
    }
};

/**
 * @brief 测试插入单个节点
 */
void testInsertSingleNode() {
    BinarySearchTree bst;
    bst.insert(5);

    assert(bst.search(5) != nullptr);
    assert(bst.search(5)->value == 5);
    assert(bst.search(3) == nullptr);

    std::cout << "testInsertSingleNode: PASSED\n";
}

/**
 * @brief 测试插入多个节点
 */
void testInsertMultipleNodes() {
    BinarySearchTree bst;
    bst.insert(5);
    bst.insert(3);
    bst.insert(7);

    assert(bst.search(5) != nullptr);
    assert(bst.search(3) != nullptr);
    assert(bst.search(7) != nullptr);
    assert(bst.search(5)->left->value == 3);
    assert(bst.search(5)->right->value == 7);

    std::cout << "testInsertMultipleNodes: PASSED\n";
}

/**
 * @brief 测试查找存在的节点
 */
void testSearchExistingNode() {
    BinarySearchTree bst;
    bst.insert(5);
    bst.insert(3);
    bst.insert(7);

    TreeNode* node = bst.search(3);
    assert(node != nullptr);
    assert(node->value == 3);

    std::cout << "testSearchExistingNode: PASSED\n";
}

/**
 * @brief 测试查找不存在的节点
 */
void testSearchNonExistingNode() {
    BinarySearchTree bst;
    bst.insert(5);
    bst.insert(3);
    bst.insert(7);

    TreeNode* node = bst.search(10);
    assert(node == nullptr);

    std::cout << "testSearchNonExistingNode: PASSED\n";
}

/**
 * @brief 测试中序遍历
 */
void testInorderTraversal() {
    BinarySearchTree bst;
    bst.insert(5);
    bst.insert(3);
    bst.insert(7);
    bst.insert(1);
    bst.insert(9);

    int result[100];
    int size;
    bst.inorderTraversal(result, size);

    assert(size == 5);
    assert(result[0] == 1);
    assert(result[1] == 3);
    assert(result[2] == 5);
    assert(result[3] == 7);
    assert(result[4] == 9);

    std::cout << "testInorderTraversal: PASSED\n";
}

/**
 * @brief 测试空树
 */
void testEmptyTree() {
    BinarySearchTree bst;

    assert(bst.search(5) == nullptr);

    int result[100];
    int size;
    bst.inorderTraversal(result, size);
    assert(size == 0);

    std::cout << "testEmptyTree: PASSED\n";
}

/**
 * @brief 主函数
 */
int main() {
    std::cout << "Running BinarySearchTree tests...\n\n";

    testInsertSingleNode();
    testInsertMultipleNodes();
    testSearchExistingNode();
    testSearchNonExistingNode();
    testInorderTraversal();
    testEmptyTree();

    std::cout << "\nAll tests passed!\n";
    return 0;
}
