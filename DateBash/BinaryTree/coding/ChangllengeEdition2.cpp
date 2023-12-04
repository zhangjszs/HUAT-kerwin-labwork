/*
 * @Description: 
 * @Version: 2.0
 * @Autor: kerwinz
 * @Date: 2023-12-02 16:54:57
 * @LastEditors: kerwinz
 * @LastEditTime: 2023-12-02 16:56:31
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义哈夫曼树的结点结构
typedef struct HuffmanNode {
    char data;                // 结点存储的字符
    int frequency;            // 字符出现的频率
    struct HuffmanNode *left;  // 左子树指针
    struct HuffmanNode *right; // 右子树指针
} HuffmanNode;

// 创建哈夫曼树结点
HuffmanNode *createHuffmanNode(char data, int frequency) {
    HuffmanNode *node = (HuffmanNode *)malloc(sizeof(HuffmanNode));
    node->data = data;
    node->frequency = frequency;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// 构建哈夫曼树
HuffmanNode *buildHuffmanTree(char *data, int *frequency, int size) {
    // 创建叶子结点，并将其插入最小堆中
    // 最小堆用于选择频率最小的两个结点
    MinHeap *minHeap = createMinHeap(size);
    for (int i = 0; i < size; i++) {
        HuffmanNode *node = createHuffmanNode(data[i], frequency[i]);
        insertMinHeap(minHeap, node);
    }

    // 不断选择频率最小的两个结点，合并为新的结点
    // 直到堆中只剩下一个结点，即哈夫曼树的根结点
    while (minHeap->size > 1) {
        HuffmanNode *node1 = deleteMinHeap(minHeap);
        HuffmanNode *node2 = deleteMinHeap(minHeap);
        HuffmanNode *mergedNode = createHuffmanNode('$', node1->frequency + node2->frequency);
        mergedNode->left = node1;
        mergedNode->right = node2;
        insertMinHeap(minHeap, mergedNode);
    }

    // 返回哈夫曼树的根结点
    return deleteMinHeap(minHeap);
}

// 递归方式打印哈夫曼编码
void printHuffmanCodes(HuffmanNode *root, char *prefix) {
    if (root->left == NULL && root->right == NULL) {
        printf("%c: %s\n", root->data, prefix);
        return;
    }

    int length = strlen(prefix);
    char *leftPrefix = (char *)malloc((length + 2) * sizeof(char));
    char *rightPrefix = (char *)malloc((length + 2) * sizeof(char));
    strcpy(leftPrefix, prefix);
    strcpy(rightPrefix, prefix);
    leftPrefix[length] = '0';
    leftPrefix[length + 1] = '\0';
    rightPrefix[length] = '1';
    rightPrefix[length + 1] = '\0';

    printHuffmanCodes(root->left, leftPrefix);
    printHuffmanCodes(root->right, rightPrefix);

    free(leftPrefix);
    free(rightPrefix);
}

// 哈夫曼编码算法
void huffmanEncoding(char *data, int *frequency, int size) {
    // 构建哈夫曼树
    HuffmanNode *root = buildHuffmanTree(data, frequency, size);

    // 打印哈夫曼编码
    char *prefix = (char *)malloc(sizeof(char));
    prefix[0] = '\0';
    printHuffmanCodes(root, prefix);

    free(prefix);
}

int main() {
    // 输入字符及其频率
    char data[] = {'a', 'b', 'c', 'd', 'e'};
    int frequency[] = {5, 9, 12, 13, 16};
    int size = sizeof(data) / sizeof(data[0]);

    // 进行哈夫曼编码
    huffmanEncoding(data, frequency, size);

    return 0;
}