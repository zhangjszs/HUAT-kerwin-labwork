
/*整数集合Ａ和集合B中各有若干个元素，请编程实现集合Ａ与集合B的相关运算，包括交集、并集、补集和差集等。
提示：集合Ａ和集合Ｂ采用单链表来表示，各种集合运算的结果也是链表。
这版本是读取文件的版本，文件名为elements_a.txt和elements_b.txt，文件中的元素之间请以空格隔开。
*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

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
    const char *fileA = "elements_a.txt";
    const char *fileB = "elements_b.txt";

    readElementsFromFile(&headA, fileA);
    readElementsFromFile(&headB, fileB);

    printf("A :\t");
    printList(headA);

    printf("B :\t");
    printList(headB);

    Node *universalSet = createUniversalSet(1, 20); // 创建全集，数字从1到20

    printf("U :\t");
    printList(universalSet);

    printf("A∩B :\t");
    Node *intersectionSet = intersection(headA, headB);
    printList(intersectionSet);

    printf("AuB :\t");
    Node *unionSetResult = unionSet(headA, headB);
    printList(unionSetResult);

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

void readElementsFromFile(struct Node **head, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("无法打开文件 %s\n", filename);
        printf("元素之间请以空格隔开\n");
        exit(1);
    }

    int data;
    char buffer[20]; // 用于存储文件中的每个单词

    while (fscanf(file, "%s", buffer) != EOF)
    {
        // 检查单词是否为数字
        int isValid = 1;
        for (int i = 0; buffer[i] != '\0'; ++i)
        {
            if (!isdigit(buffer[i]))
            {
                isValid = 0;
                break;
            }
        }

        if (isValid)
        {
            data = atoi(buffer);
            insertNode(head, data);
        }
        else
        {
            printf("文件%s中包含非数字元素: %s\n", filename,buffer);
        }
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
