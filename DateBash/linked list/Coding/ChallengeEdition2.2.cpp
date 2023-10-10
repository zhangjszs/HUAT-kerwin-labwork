
/*整数集合Ａ和集合B中各有若干个元素，请编程实现集合Ａ与集合B的相关运算，包括交集、并集、补集和差集等。
提示：集合Ａ和集合Ｂ采用单链表来表示，各种集合运算的结果也是链表。
使用链表来实现 但是是手动输入
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node
{
    int data;
    struct Node *next;
} Node;

Node *createNode(int data);
void insertNode(Node **head, int data);
void printList(Node *head);
Node *intersection(Node *headA, Node *headB);
Node *unionSet(Node *headA, Node *headB);
Node *complement(Node *headA, Node *headB);
int readElementFromInput();
void readElementsFromInput(struct Node** head);

int main()
{
    Node *headA = NULL;
    Node *headB = NULL;
    char input[10];
    int value;

    // 在集合 A 中插入元素
    printf("Enter elements for Set A:\n");
    readElementsFromInput(&headA);

    // 在集合 B 中插入元素
    printf("Enter elements for Set B:\n");
    readElementsFromInput(&headB);

    printf("A :\t");
    printList(headA);

    printf("B :\t");
    printList(headB);

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

    // 释放链表内存
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

    free(intersectionSet);
    free(unionSetResult);
    free(complementSetA);
    free(complementSetB);

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
            insertNode(&result, headA->data);
            if (tail == NULL)
            {
                tail = result;
            }
            else
            {
                tail = tail->next;
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

    while (headA != NULL)
    {
        insertNode(&result, headA->data);
        if (tail == NULL)
        {
            tail = result;
        }
        else
        {
            tail = tail->next;
        }
        headA = headA->next;
    }

    while (headB != NULL)
    {
        int isDuplicate = 0;
        Node *temp = result;
        while (temp != NULL)
        {
            if (temp->data == headB->data)
            {
                isDuplicate = 1;
                break;
            }
            temp = temp->next;
        }
        if (!isDuplicate)
        {
            insertNode(&result, headB->data);
            if (tail == NULL)
            {
                tail = result;
            }
            else
            {
                tail = tail->next;
            }
        }
        headB = headB->next;
    }

    return result;
}

Node *complement(Node *headA, Node *headB)
{
    Node *result = NULL;
    Node *tail = NULL;

    while (headA != NULL)
    {
        int isInB = 0;
        Node *temp = headB;
        while (temp != NULL)
        {
            if (temp->data == headA->data)
            {
                isInB = 1;
                break;
            }
            temp = temp->next;
        }
        if (!isInB)
        {
            insertNode(&result, headA->data);
            if (tail == NULL)
            {
                tail = result;
            }
            else
            {
                tail = tail->next;
            }
        }
        headA = headA->next;
    }

    return result;
}

int readElementFromInput() {
    char input[100];
    printf("Enter an element (enter 'end' to finish): ");
    scanf("%s", input);
    if (strcmp(input, "end") == 0) {
        return -1; // 表示输入结束
    }
    int value;
    if (sscanf(input, "%d", &value) != 1) {
        printf("Invalid input. Please enter a number or 'end' to finish.\n");
        return readElementFromInput(); // 递归调用直到输入有效数字或者'end'
    }
    return value;
}

void readElementsFromInput(struct Node** head) {
    int value;
    while ((value = readElementFromInput()) != -1) {
        insertNode(head, value);
    }
}