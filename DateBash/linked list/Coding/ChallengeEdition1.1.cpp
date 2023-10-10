/*
 * @Description:
 * @Version: 2.0
 * @Autor: kerwinz
 * @Date: 2023-10-09 23:17:01
 * @LastEditors: kerwinz
 * @LastEditTime: 2023-10-10 10:47:04
 */
/*
设LA和LB是两个有序的单链表，Pa和Pb分别指向两个表的表头结点，试编写一个算法将这两个表归并为一个有序的单链表。
LA和LB来着用户输入
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义单链表节点结构
typedef struct Node
{
    int data;
    struct Node *next;
} Node;

Node *createNode(int data);
void insertNode(Node **head, int data);
void printList(Node *head);
Node *mergeList(Node *headA, Node *headB);
int readElementFromInput();
void readElementsFromInput(struct Node **head);
void quickSort(Node **head);

int main(int argc, char const *argv[])
{
    Node *headA = NULL;
    Node *headB = NULL;
    char input[10];
    int value;

    printf("Enter elements for Set A:\n");
    readElementsFromInput(&headA);

    printf("Enter elements for Set B:\n");
    readElementsFromInput(&headB);

    // 对两个链表进行合并
    Node *mergedList = mergeList(headA, headB);
    // printf("headA :\t");
    // printList(headA);
    // printf("headB :\t");
    // printList(headB);
    // 打印排序前的链表
    printf("before sort mergedList :\t");
    printList(mergedList);
    // 对合并后的链表进行排序
    quickSort(&mergedList);
    // 打印排序后的链表
    printf("after sort mergedList :\t");
    printList(mergedList);

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

// Node *mergeList(Node *headA, Node *headB)
// {
//     Node *result = NULL;
//     Node *tail = NULL;

//     while (headA != NULL && headB != NULL)
//     {
//         if (headA->data < headB->data)
//         {
//             headA = headA->next;
//         }
//         else if (headA->data > headB->data)
//         {
//             headB = headB->next;
//         }
//         else
//         {
//             Node *newNode = createNode(headA->data);
//             if (result == NULL)
//             {
//                 result = newNode;
//                 tail = newNode;
//             }
//             else
//             {
//                 tail->next = newNode;
//                 tail = newNode;
//             }
//             headA = headA->next;
//             headB = headB->next;
//         }
//     }
//     return result;
// }

Node *mergeList(Node *headA, Node *headB)
{
    if (headA == NULL)
    {
        return headB;
    }
    if (headB == NULL)
    {
        return headA;
    }

    // 找到链表A的尾部节点
    Node *current = headA;
    while (current->next != NULL)
    {
        current = current->next;
    }

    // 将链表A的尾部节点的next指向链表B的表头节点
    current->next = headB;

    return headA;
}


int readElementFromInput()
{
    char input[100];
    printf("Enter an element (enter 'end' to finish): ");
    scanf("%s", input);
    if (strcmp(input, "end") == 0)
    {
        return -1; // 表示输入结束
    }
    int value;
    if (sscanf(input, "%d", &value) != 1)
    {
        printf("Invalid input. Please enter a number or 'end' to finish.\n");
        return readElementFromInput(); // 递归调用直到输入有效数字或者'end'
    }
    return value;
}

void readElementsFromInput(Node **head)
{
    int value;
    while ((value = readElementFromInput()) != -1)
    {
        insertNode(head, value);
    }
}

void quickSort(Node **head)
{
    if (*head == NULL || (*head)->next == NULL)
    {
        return;
    }

    Node *pivot = *head;
    Node *temp = (*head)->next;
    Node *left = NULL;
    Node *right = NULL;

    while (temp != NULL)
    {
        Node *next = temp->next;
        if (temp->data < pivot->data)
        {
            temp->next = left;
            left = temp;
        }
        else
        {
            temp->next = right;
            right = temp;
        }
        temp = next;
    }

    quickSort(&left);
    quickSort(&right);

    if (left == NULL)
    {
        *head = pivot;
    }
    else
    {
        *head = left;
        Node *tail = left;
        while (tail->next != NULL)
        {
            tail = tail->next;
        }
        tail->next = pivot;
    }

    pivot->next = right;
}
