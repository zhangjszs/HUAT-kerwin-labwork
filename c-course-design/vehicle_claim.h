#ifndef VEHICLE_CLAIM_H
#define VEHICLE_CLAIM_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// 常量定义
#define MAXNAME 30
#define MAXDAY 20
#define MAXID 20
#define TRUE 1
#define FALSE 0

// 索赔信息结构体
typedef struct ClimeData {
    char ServiceStationName[MAXNAME]; // 服务站名称
    char id[MAXID];                  // 索赔编号
    int ClaimAmount;                 // 索赔金额
    char ClaimTime[MAXDAY];          // 索赔日期
    char CarVIN[MAXID];              // 底盘号
    char Reviewer[MAXNAME];          // 审核人
} ClimeData;

// 链表节点结构体
typedef struct Node {
    ClimeData data;
    struct Node *next;
} Node;

// 函数声明
void welcome(void);
void makeMenu(void);
void keyDown(void);
void getNowTime(void);

// 链表操作函数
Node *createHead(void);
Node *createNode(ClimeData data);
void insertNodeByHead(Node *headNode, ClimeData data);
void insertNodeByTail(Node *headNode, ClimeData data);
void deleteNodeByClimeID(Node *headNode, char *DataId);

// 查找函数
Node *searchByCarVIN(Node *headNode, char *CarVIN);
Node *searchByCarID(Node *headNode, char *ID);
Node *checkRepeatID(Node *headNode, char *ID);
void searchAndPrintByCarVIN(Node *headNode, char *CarVIN);

// 数据处理函数
void bubbleSortList(Node *headNode);
void printList(Node *headNode);

// 验证函数
int checkID(char *ID);
int checkTrueTime(char *time);
int IsLeapYear(int year);

// 统计函数
int statisticalAmountByName(Node *headNode, char *name);
int statisticalAmountByReviewer(Node *headNode, char *name);
int statisticalAmountByTime(Node *headNode, char *Time);

// 文件操作函数
void saveInfoFile(const char *fileName, Node *headNode);
void readInfoFile(const char *fileName, Node *headNode);

// 辅助函数
void substr(char dest[], char src[], int pos, int length);

// 内存管理函数
void freeList(Node *headNode);

#endif // VEHICLE_CLAIM_H