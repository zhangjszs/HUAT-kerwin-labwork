/*
 * @Description:
 * @Version: 2.0
 * @Autor: kerwinz
 * @Date: 2023-10-05 11:22:58
 * @LastEditors: kerwinz
 * @LastEditTime: 2023-10-05 14:55:49
 */
/*使用数组来实现
#include <stdio.h>

void intersection(const int A[], int nA, const int B[], int nB);
void unionSet(const int A[], int nA, const int B[], int nB);
void complement(const int A[], int nA, const int B[], int nB);

int main() {
    int A[] = {1, 2, 3, 4, 5};
    int nA = sizeof(A) / sizeof(A[0]);
    int B[] = {3, 4, 5, 6, 7};
    int nB = sizeof(B) / sizeof(B[0]);

    printf("集合A：");
    for (int i = 0; i < nA; i++) {
        printf("%d ", A[i]);
    }
    printf("\n");

    printf("集合B：");
    for (int i = 0; i < nB; i++) {
        printf("%d ", B[i]);
    }
    printf("\n");

    printf("交集：");
    intersection(A, nA, B, nB);

    printf("并集：");
    unionSet(A, nA, B, nB);

    printf("A的补集：");
    complement(A, nA, B, nB);

    printf("B的补集：");
    complement(B, nB, A, nA);

    return 0;
}

void intersection(const int A[], int nA, const int B[], int nB) {
    for (int i = 0; i < nA; i++) {
        for (int j = 0; j < nB; j++) {
            if (A[i] == B[j]) {
                printf("%d ", A[i]);
                break;
            }
        }
    }
    printf("\n");
}

void unionSet(const int A[], int nA, const int B[], int nB) {
    for (int i = 0; i < nA; i++) {
        printf("%d ", A[i]);
    }
    for (int i = 0; i < nB; i++) {
        int isDuplicate = 0;
        for (int j = 0; j < nA; j++) {
            if (B[i] == A[j]) {
                isDuplicate = 1;
                break;
            }
        }
        if (!isDuplicate) {
            printf("%d ", B[i]);
        }
    }
    printf("\n");
}

void complement(const int A[], int nA, const int B[], int nB) {
    for (int i = 0; i < nA; i++) {
        int isInB = 0;
        for (int j = 0; j < nB; j++) {
            if (A[i] == B[j]) {
                isInB = 1;
                break;
            }
        }
        if (!isInB) {
            printf("%d ", A[i]);
        }
    }
    printf("\n");
}
使用链表来实现 但是是手动输入
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node* createNode(int data);
void insertNode(Node** head, int data);
void printList(Node* head);
Node* intersection(Node* headA, Node* headB);
Node* unionSet(Node* headA, Node* headB);
Node* complement(Node* headA, Node* headB);

int main() {
    Node* headA = NULL;
    Node* headB = NULL;

    // 在集合 A 中插入元素
    insertNode(&headA, 1);
    insertNode(&headA, 2);
    insertNode(&headA, 3);
    insertNode(&headA, 4);
    insertNode(&headA, 5);

    // 在集合 B 中插入元素
    insertNode(&headB, 3);
    insertNode(&headB, 4);
    insertNode(&headB, 5);
    insertNode(&headB, 6);
    insertNode(&headB, 7);

    printf("集合 A：");
    printList(headA);

    printf("集合 B：");
    printList(headB);

    printf("交集：");
    Node* intersectionSet = intersection(headA, headB);
    printList(intersectionSet);

    printf("并集：");
    Node* unionSetResult = unionSet(headA, headB);
    printList(unionSetResult);

    printf("A的补集：");
    Node* complementSetA = complement(headA, headB);
    printList(complementSetA);

    printf("B的补集：");
    Node* complementSetB = complement(headB, headA);
    printList(complementSetB);

    // 释放链表内存
    while (headA != NULL) {
        Node* temp = headA;
        headA = headA->next;
        free(temp);
    }

    while (headB != NULL) {
        Node* temp = headB;
        headB = headB->next;
        free(temp);
    }

    free(intersectionSet);
    free(unionSetResult);
    free(complementSetA);
    free(complementSetB);

    return 0;
}

Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("内存分配失败\n");
        exit(1);
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void insertNode(Node** head, int data) {
    Node* newNode = createNode(data);
    if (*head == NULL) {
        *head = newNode;
    } else {
        Node* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

void printList(Node* head) {
    Node* temp = head;
    while (temp != NULL) {
        printf("%d ", temp->data);
        temp = temp->next;
    }
    printf("\n");
}

Node* intersection(Node* headA, Node* headB) {
    Node* result = NULL;
    Node* tail = NULL;

    while (headA != NULL && headB != NULL) {
        if (headA->data < headB->data) {
            headA = headA->next;
        } else if (headA->data > headB->data) {
            headB = headB->next;
        } else {
            insertNode(&result, headA->data);
            if (tail == NULL) {
                tail = result;
            } else {
                tail = tail->next;
            }
            headA = headA->next;
            headB = headB->next;
        }
    }

    return result;
}

Node* unionSet(Node* headA, Node* headB) {
    Node* result = NULL;
    Node* tail = NULL;

    while (headA != NULL) {
        insertNode(&result, headA->data);
        if (tail == NULL) {
            tail = result;
        } else {
            tail = tail->next;
        }
        headA = headA->next;
    }

    while (headB != NULL) {
        int isDuplicate = 0;
        Node* temp = result;
        while (temp != NULL) {
            if (temp->data == headB->data) {
               ```c
                isDuplicate = 1;
                break;
            }
            temp = temp->next;
        }
        if (!isDuplicate) {
            insertNode(&result, headB->data);
            if (tail == NULL) {
                tail = result;
            } else {
                tail = tail->next;
            }
        }
        headB = headB->next;
    }

    return result;
}

Node* complement(Node* headA, Node* headB) {
    Node* result = NULL;
    Node* tail = NULL;

    while (headA != NULL) {
        int isInB = 0;
        Node* temp = headB;
        while (temp != NULL) {
            if (temp->data == headA->data) {
                isInB = 1;
                break;
            }
            temp = temp->next;
        }
        if (!isInB) {
            insertNode(&result, headA->data);
            if (tail == NULL) {
                tail = result;
            } else {
                tail = tail->next;
            }
        }
        headA = headA->next;
    }

    return result;
}
*/
#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int data;
    struct Node *next;
} Node;

Node *createNode(int data);
void insertNode(Node **head, int data);
void printList(Node *head);
void readElementsFromFile(Node **head, const char *filename);
Node *intersection(Node *headA, Node *headB);
Node *unionSet(Node *headA, Node *headB);
Node *complement(Node *headA, Node *headB);
Node *createUniversalSet(int start, int end);

int main()
{
    Node *headA = NULL;
    Node *headB = NULL;
    const char *fileA = "E:\\coding things\\vscodegitcpp\\withoutmake\\SchoolDatebash\\linked list\\elements_a.txt";
    const char *fileB = "E:\\coding things\\vscodegitcpp\\withoutmake\\SchoolDatebash\\linked list\\elements_b.txt";

    readElementsFromFile(&headA, fileA);
    readElementsFromFile(&headB, fileB);

    printf("A :\t");
    printList(headA);

    printf("B :\t");
    printList(headB);

    Node *universalSet = createUniversalSet(1, 20); // 创建全集，数字从1到20

    printf("U :\t");
    printList(universalSet);

    printf("A-B :\t");
    Node *complementSetA = complement(headA, headB);
    printList(complementSetA);

    printf("B-A :\t");
    Node *complementSetB = complement(headB, headA);
    printList(complementSetB);

    printf("U-A :\t"); // A的补集
    Node *complementSetA2 = complement(universalSet, headA);
    printList(complementSetA2);

    printf("U-B :\t"); // B的补集
    Node *complementSetB2 = complement(universalSet, headB);
    printList(complementSetB2);

    // 释放内存
    while (headA != NULL)
    {
        Node *temp = headA;
        headA = headA->next;
        free(temp);
    }

    while (headB != NULL)
    {
        Node *temp = headB;
        headB = headB->next;
        free(temp);
    }

    while (complementSetA != NULL)
    {
        Node *temp = complementSetA;
        complementSetA = complementSetA->next;
        free(temp);
    }

    while (complementSetB != NULL)
    {
        Node *temp = complementSetB;
        complementSetB = complementSetB->next;
        free(temp);
    }

    // 释放全集的内存
    while (universalSet != NULL)
    {
        Node *temp = universalSet;
        universalSet = universalSet->next;
        free(temp);
    }

    while (complementSetA2 != NULL)
    {
        Node *temp = complementSetA2;
        complementSetA2 = complementSetA2->next;
        free(temp);
    }

    while (complementSetB2 != NULL)
    {
        Node *temp = complementSetB2;
        complementSetB2 = complementSetB2->next;
        free(temp);
    }

    return 0;
}

Node *createNode(int data)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL)
    {
        printf("内存分配失败\n");
        exit(1);
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void insertNode(Node **head, int data)
{
    Node *newNode = createNode(data);
    if (*head == NULL)
    {
        *head = newNode;
    }
    else
    {
        Node *temp = *head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

void printList(Node *head)
{
    Node *temp = head;
    while (temp != NULL)
    {
        printf("%d ", temp->data);
        temp = temp->next;
    }
    printf("\n");
}

void readElementsFromFile(Node **head, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("无法打开文件 %s\n", filename);
        exit(1);
    }

    int data;
    while (fscanf(file, "%d", &data) != EOF)
    {
        insertNode(head, data);
    }

    fclose(file);
}

Node *intersection(Node *headA, Node *headB)
{
    Node *result = NULL;
    Node *tail = NULL;

    while (headA != NULL && headB != NULL)
    {
        if (headA->data < headB->data)
        {
            headA = headA->next;
        }
        else if (headA->data > headB->data)
        {
            headB = headB->next;
        }
        else
        {
            Node *newNode = createNode(headA->data);
            if (result == NULL)
            {
                result = newNode;
                tail = newNode;
            }
            else
            {
                tail->next = newNode;
                tail = newNode;
            }
            headA = headA->next;
            headB = headB->next;
        }
    }

    return result;
}

Node *unionSet(Node *headA, Node *headB)
{
    Node *result = NULL;
    Node *tail = NULL;

    while (headA != NULL && headB != NULL)
    {
        if (headA->data < headB->data)
        {
            Node *newNode = createNode(headA->data);
            if (result == NULL)
            {
                result = newNode;
                tail = newNode;
            }
            else
            {
                tail->next = newNode;
                tail = newNode;
            }
            headA = headA->next;
        }
        else if (headA->data > headB->data)
        {
            Node *newNode = createNode(headB->data);
            if (result == NULL)
            {
                result = newNode;
                tail = newNode;
            }
            else
            {
                tail->next = newNode;
                tail = newNode;
            }
            headB = headB->next;
        }
        else
        {
            Node *newNode = createNode(headA->data);
            if (result == NULL)
            {
                result = newNode;
                tail = newNode;
            }
            else
            {
                tail->next = newNode;
                tail = newNode;
            }
            headA = headA->next;
            headB = headB->next;
        }
    }

    while (headA != NULL)
    {
        Node *newNode = createNode(headA->data);
        if (result == NULL)
        {
            result = newNode;
            tail = newNode;
        }
        else
        {
            tail->next = newNode;
            tail = newNode;
        }
        headA = headA->next;
    }

    while (headB != NULL)
    {
        Node *newNode = createNode(headB->data);
        if (result == NULL)
        {
            result = newNode;
            tail = newNode;
        }
        else
        {
            tail->next = newNode;
            tail = newNode;
        }
        headB = headB->next;
    }

    return result;
}

Node *complement(Node *headA, Node *headB)
{
    Node *result = NULL;
    Node *tail = NULL;

    while (headA != NULL && headB != NULL)
    {
        if (headA->data < headB->data)
        {
            Node *newNode = createNode(headA->data);
            if (result == NULL)
            {
                result = newNode;
                tail = newNode;
            }
            else
            {
                tail->next = newNode;
                tail = newNode;
            }
            headA = headA->next;
        }
        else if (headA->data > headB->data)
        {
            headB = headB->next;
        }
        else
        {
            headA = headA->next;
            headB = headB->next;
        }
    }

    while (headA != NULL)
    {
        Node *newNode = createNode(headA->data);
        if (result == NULL)
        {
            result = newNode;
            tail = newNode;
        }
        else
        {
            tail->next = newNode;
            tail = newNode;
        }
        headA = headA->next;
    }

    return result;
}

Node *createUniversalSet(int start, int end)
{
    Node *universalSet = NULL;
    for (int i = start; i <= end; i++)
    {
        insertNode(&universalSet, i);
    }
    return universalSet;
}
