/*
 * @Author: kerwin-win zhangjszs@foxmail.com
 * @Date: 2024-01-01 00:00:00
 * @LastEditors: kerwin-win zhangjszs@foxmail.com
 * @LastEditTime: 2024-01-01 00:00:00
 * @FilePath: \HUAT-kerwin-labwork\C语言课设\vehicle_claim.c
 * @Description: 车辆索赔管理系统主程序
 *
 * 功能：实现车辆索赔信息的增删改查、统计和文件持久化
 * 数据结构：使用链表存储索赔记录
 * 特性：控制台界面、文件I/O、数据验证
 */

#include "vehicle_claim.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// 常量定义
#define MAXNAME 30
#define MAXDAY 20
#define MAXID 20
#define TRUE 1
#define FALSE 0

// 全局变量
struct Node* list = NULL;  // 使用全局链表
int Nowyear, Nowmonth, Nowday;
int monthday[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// 索赔信息结构体
struct ClimeData {
    char ServiceStationName[MAXNAME];  // 服务站名称
    char id[MAXID];                    // 索赔编号
    int ClaimAmount;                   // 索赔金额
    char ClaimTime[MAXDAY];            // 索赔日期
    char CarVIN[MAXID];                // 底盘号
    char Reviewer[MAXNAME];            // 审核人
};

// 链表节点结构体
struct Node {
    struct ClimeData data;
    struct Node* next;
};

// 函数声明
void welcome(void);
void makeMenu(void);
void keyDown(void);
void getNowTime(void);

// 链表操作函数
struct Node* createHead(void);
struct Node* createNode(struct ClimeData data);
void insertNodeByHead(struct Node* headNode, struct ClimeData data);
void insertNodeByTail(struct Node* headNode, struct ClimeData data);
void deleteNodeByClimeID(struct Node* headNode, char* DataId);

// 查找函数
struct Node* searchByCarVIN(struct Node* headNode, char* CarVIN);
struct Node* searchByCarID(struct Node* headNode, char* ID);
struct Node* checkRepeatID(struct Node* headNode, char* ID);
void searchAndPrintByCarVIN(struct Node* headNode, char* CarVIN);

// 数据处理函数
void bubbleSortList(struct Node* headNode);
void printList(struct Node* headNode);

// 验证函数
int checkID(char* ID);
int checkTrueTime(char* time);
int IsLeapYear(int year);

// 统计函数
int statisticalAmountByName(struct Node* headNode, char* name);
int statisticalAmountByReviewer(struct Node* headNode, char* name);
int statisticalAmountByTime(struct Node* headNode, char* Time);

// 文件操作函数
void saveInfoFile(const char* fileName, struct Node* headNode);
void readInfoFile(const char* fileName, struct Node* headNode);

// 辅助函数
void substr(char dest[], char src[], int pos, int length);

// 内存管理函数
void freeList(struct Node* headNode);
/**
 * @brief 显示欢迎界面
 *
 * 打印系统欢迎信息和制作者信息，等待用户按Enter键进入
 */
void welcome(void) {
    system("color F0");
    printf("\n\n\n\n");
    printf("\t\t ********************************************************************\n");
