#include "vehicle_claim.h"
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

// 全局变量
struct Node *list = NULL; // 使用全局链表
int Nowyear, Nowmonth, Nowday;
int monthday[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// 索赔信息结构体
struct ClimeData
{
	char ServiceStationName[MAXNAME]; // 服务站名称
	char id[MAXID];					  // 索赔编号
	int ClaimAmount;				  // 索赔金额
	char ClaimTime[MAXDAY];			  // 索赔日期
	char CarVIN[MAXID];				  // 底盘号
	char Reviewer[MAXNAME];			  // 审核人
};

// 链表节点结构体
struct Node
{
	struct ClimeData data;
	struct Node *next;
};

// 函数声明
void welcome(void);
void makeMenu(void);
void keyDown(void);
void getNowTime(void);

// 链表操作函数
struct Node *createHead(void);
struct Node *createNode(struct ClimeData data);
void insertNodeByHead(struct Node *headNode, struct ClimeData data);
void insertNodeByTail(struct Node *headNode, struct ClimeData data);
void deleteNodeByClimeID(struct Node *headNode, char *DataId);

// 查找函数
struct Node *searchByCarVIN(struct Node *headNode, char *CarVIN);
struct Node *searchByCarID(struct Node *headNode, char *ID);
struct Node *checkRepeatID(struct Node *headNode, char *ID);
void searchAndPrintByCarVIN(struct Node *headNode, char *CarVIN);

// 数据处理函数
void bubbleSortList(struct Node *headNode);
void printList(struct Node *headNode);

// 验证函数
int checkID(char *ID);
int checkTrueTime(char *time);
int IsLeapYear(int year);

// 统计函数
int statisticalAmountByName(struct Node *headNode, char *name);
int statisticalAmountByReviewer(struct Node *headNode, char *name);
int statisticalAmountByTime(struct Node *headNode, char *Time);

// 文件操作函数
void saveInfoFile(const char *fileName, struct Node *headNode);
void readInfoFile(const char *fileName, struct Node *headNode);

// 辅助函数
void substr(char dest[], char src[], int pos, int length);

// 内存管理函数
void freeList(struct Node *headNode);
void welcome(void)
{
	system("color F0");
	printf("\n\n\n\n");
	printf("\t\t ********************************************************************\n");
	printf("\t\t ********************************************************************\n");
	printf("\t\t ********************************************************************\n");
	printf("\t\t **                                                               **\n");
	printf("\t\t **                                                               **\n");
	printf("\t\t **   欢   迎   使   用   汽   车   索   赔   管   理   系   统 !  **\n");
	printf("\t\t **                                                               **\n");
	printf("\t\t **                                                               **\n");
	printf("\t\t **                                                               **\n");
	printf("\t\t **                                                               **\n");
	printf("\t\t **                                                               **\n");
	printf("\t\t **   制   作   人：章崇文       指   导   教   师：史旅华 齐心     **\n");
	printf("\t\t **   学校 湖北汽车工业学院      班级 计算机 222                  **\n");
	printf("\t\t ********************************************************************\n");
	printf("\t\t *************   请   按   ENTER   键   进   入   系   统************\n");
	printf("\t\t ********************************************************************\n");

	while (getchar() != '\n')
		; // 清空输入缓冲区
	system("cls");
}
void makeMenu(void) // 菜单界面
{
	printf("\n\n\t\t--------------------成 功 进 入 系 统 !--------------------\n");
	printf("\t\t--------------------【以 下 是 菜 单：】--------------------\n");
	printf("\t\t---------------        1.0读取课设要求       ---------------\n");
	printf("\t\t---------------        2.0查询索赔信息       ---------------\n");
	printf("\t\t---------------        3.0查询车辆历史索赔   ---------------\n");
	printf("\t\t---------------        4.0修改索赔信息       ---------------\n");
	printf("\t\t---------------        5.0删除索赔信息       ---------------\n");
	printf("\t\t---------------        6.0添加索赔信息       ---------------\n");
	printf("\t\t---------------        7.0查看统计信息       ---------------\n");
	printf("\t\t---------------        8.0退出该程序         ---------------\n");
	printf("\t\t------------------------------------------------------------\n");
	printf("\t\t----如有问题到 https://www.cnblogs.com/jszs0013/进行留言----\n");
}
// 欢迎界面
struct Node *createHead(void)
{
	struct Node *headNode = (struct Node *)malloc(sizeof(struct Node)); // 动态内存申请
	if (headNode == NULL)
	{
		printf("内存分配失败！\n");
		exit(1);
	}
	headNode->next = NULL;
	return headNode;
}
// 创建节点为数据做准备
struct Node *createNode(struct ClimeData data)
{
	struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
	if (newNode == NULL)
	{
		printf("内存分配失败！\n");
		exit(1);
	}
	newNode->data = data;
	newNode->next = NULL;
	return newNode;
}
// 插入数据 使用表头法插入
void insertNodeByHead(struct Node *headNode, struct ClimeData data)
{
	if (headNode == NULL)
		return;

	struct Node *newNode = createNode(data);
	newNode->next = headNode->next;
	headNode->next = newNode;
}
// 插入数据 使用表尾法
void insertNodeByTail(struct Node *headNode, struct ClimeData data)
{
	if (headNode == NULL)
		return;

	struct Node *pMove = headNode;
	while (pMove->next != NULL) // 修复：原来的条件会导致空指针访问
	{
		pMove = pMove->next;
	}
	struct Node *newNode = createNode(data);
	pMove->next = newNode;
}
// 指定位置删除，给定索赔编号，删除索赔信息
void deleteNodeByClimeID(struct Node *headNode, char *DataId)
{
	if (headNode == NULL || headNode->next == NULL)
	{
		printf("链表为空或没有数据节点!\n");
		return;
	}

	struct Node *posLeftNode = headNode; // 修复：从头节点开始
	struct Node *posNode = headNode->next;

	while (posNode != NULL && strcmp(posNode->data.id, DataId) != 0) // 修复：添加!=0
	{
		posLeftNode = posNode;
		posNode = posNode->next; // 修复：应该是posNode->next
	}

	// 讨论查找结果
	if (posNode == NULL)
	{
		printf("未找到指定编号的索赔信息!\n");
		return;
	}
	else
	{
		printf("删除成功！\n");
		posLeftNode->next = posNode->next;
		free(posNode);
	}
}
// 用底盘号查找
struct Node *searchByCarVIN(struct Node *headNode, char *CarVIN)
{
	if (headNode == NULL)
		return NULL;

	struct Node *posNode = headNode->next;
	while (posNode != NULL && strcmp(posNode->data.CarVIN, CarVIN) != 0)
	{
		posNode = posNode->next;
	}
	return posNode;
}
// 通过底盘号查历史
void searchAndPrintByCarVIN(struct Node *headNode, char *CarVIN)
{
	if (headNode == NULL)
	{
		printf("链表为空!\n");
		return;
	}

	struct Node *posNode = headNode->next;
	int found = 0;

	for (; posNode != NULL; posNode = posNode->next)
	{
		if (strcmp(posNode->data.CarVIN, CarVIN) == 0)
		{
			printf("\n服务站名称: %s\n", posNode->data.ServiceStationName);
			printf("索赔编号: %s\n", posNode->data.id);
			printf("索赔日期: %s\n", posNode->data.ClaimTime);
			printf("索赔金额: %d\n", posNode->data.ClaimAmount);
			printf("底盘号: %s\n", posNode->data.CarVIN);
			printf("审核人: %s\n", posNode->data.Reviewer);
			found = 1;
		}
	}

	if (!found)
	{
		printf("未找到该底盘号的索赔记录!\n");
	}
}
// 用编号查找
struct Node *searchByCarID(struct Node *headNode, char *ID)
{
	if (headNode == NULL)
		return NULL;

	struct Node *posNode = headNode->next;
	while (posNode != NULL && strcmp(posNode->data.id, ID) != 0)
	{
		posNode = posNode->next;
	}
	return posNode;
}

struct Node *checkRepeatID(struct Node *headNode, char *ID) // 判断编号是否重复
{
	if (headNode == NULL)
		return NULL;

	struct Node *posNode = headNode->next;
	while (posNode != NULL && strcmp(posNode->data.id, ID) != 0)
	{
		posNode = posNode->next;
	}
	return posNode;
}
// 数据排序按索赔价格(冒泡排序小到大)
void bubbleSortList(struct Node *headNode)
{
	for (struct Node *p = headNode->next; p != NULL; p = p->next)
	{
		for (struct Node *q = headNode->next; q->next != NULL; q = q->next)
		{
			if (q->data.ClaimAmount > q->next->data.ClaimAmount)
			{
				struct ClimeData tempData = q->data;
				q->data = q->next->data;
				q->next->data = tempData;
			}
		}
	}
}
// 检测编号是否符合要求
int checkID(char *ID)
{
	int len = strlen(ID);
	if (len == 5)
		return 1;
	else
		return 0;
}
void substr(char dest[], char src[], int pos, int length) // 判断合法日期的函数之一
{
	strncpy(dest, &src[pos], length);
	dest[length] = '\0';
}
int IsLeapYear(int year) // 判断年份是否是润年
{
	if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
		return 1;
	else
		return 0;
}
void getNowTime(void) // 通过函数得到当前时间
{
	time_t timep;
	time(&timep);
	struct tm *p;
	p = localtime(&timep); // 修复：使用localtime而不是gmtime
	Nowyear = (1900 + p->tm_year);
	Nowmonth = 1 + p->tm_mon;
	Nowday = p->tm_mday;
}
int checkTrueTime(char *time) // 判断时间合法性
{
	if (time == NULL || strlen(time) != 10)
	{
		return FALSE;
	}

	// 检查格式是否为 yyyy-mm-dd
	if (time[4] != '-' || time[7] != '-')
	{
		return FALSE;
	}

	char ID[15];
	strncpy(ID, time, 14); // 安全复制
	ID[14] = '\0';

	char syear[10], smonth[10], sday[10];
	substr(syear, ID, 0, 4);
	substr(smonth, ID, 5, 2);
	substr(sday, ID, 8, 2);

	int putyear = atoi(syear);
	int putmonth = atoi(smonth);
	int putday = atoi(sday);

	// 检查年份有效性
	if (putyear <= 0 || putyear > Nowyear)
	{
		return FALSE;
	}

	// 检查月份有效性
	if (putmonth <= 0 || putmonth > 12)
	{
		return FALSE;
	}

	// 更新闰年天数
	if (IsLeapYear(putyear))
	{
		monthday[2] = 29;
	}
	else
	{
		monthday[2] = 28;
	}

	// 检查日期有效性
	if (putday <= 0 || putday > monthday[putmonth])
	{
		return FALSE;
	}

	// 检查是否不晚于当前日期
	if (putyear == Nowyear)
	{
		if (putmonth > Nowmonth || (putmonth == Nowmonth && putday > Nowday))
		{
			return FALSE;
		}
	}

	return TRUE;
}
// 通过服务站统计金额
int statisticalAmountByName(struct Node *headNode, char *name)
{
	if (headNode == NULL || name == NULL)
		return 0;

	int amount = 0;
	struct Node *posNode = headNode->next;
	while (posNode != NULL)
	{
		if (strcmp(posNode->data.ServiceStationName, name) == 0)
		{
			amount += posNode->data.ClaimAmount;
		}
		posNode = posNode->next;
	}
	return amount;
}

// 通过审核员统计金额
int statisticalAmountByReviewer(struct Node *headNode, char *name)
{
	if (headNode == NULL || name == NULL)
		return 0;

	int amount = 0;
	struct Node *posNode = headNode->next;
	while (posNode != NULL)
	{
		if (strcmp(posNode->data.Reviewer, name) == 0)
		{
			amount += posNode->data.ClaimAmount;
		}
		posNode = posNode->next;
	}
	return amount;
}

// 通过时间统计金额
int statisticalAmountByTime(struct Node *headNode, char *Time)
{
	if (headNode == NULL || Time == NULL)
		return 0;

	int amount = 0;
	struct Node *posNode = headNode->next;
	while (posNode != NULL)
	{
		if (strcmp(posNode->data.ClaimTime, Time) == 0)
		{
			amount += posNode->data.ClaimAmount;
		}
		posNode = posNode->next;
	}
	return amount;
}
// 文件操作 写入文件
void saveInfoFile(const char *fileName, struct Node *headNode)
{
	if (fileName == NULL || headNode == NULL)
	{
		printf("保存文件失败：参数无效!\n");
		return;
	}

	FILE *fp = fopen(fileName, "w");
	if (fp == NULL)
	{
		printf("无法创建或打开文件 %s!\n", fileName);
		return;
	}

	struct Node *pMove = headNode->next;
	fprintf(fp, "服务站名称\t索赔编号\t索赔日期\t索赔金额\t底盘号\t审核人\n");

	while (pMove != NULL)
	{
		fprintf(fp, "%s\t%s\t%s\t%d\t%s\t%s\n",
				pMove->data.ServiceStationName,
				pMove->data.id,
				pMove->data.ClaimTime,
				pMove->data.ClaimAmount,
				pMove->data.CarVIN,
				pMove->data.Reviewer);
		pMove = pMove->next;
	}

	fclose(fp);
	printf("数据已成功保存到文件 %s\n", fileName);
}
// 文件读操作
void readInfoFile(const char *fileName, struct Node *headNode)
{
	if (fileName == NULL || headNode == NULL)
	{
		printf("读取文件失败：参数无效!\n");
		return;
	}

	FILE *fp = fopen(fileName, "r");
	if (fp == NULL) // 文件不存在就创建文件
	{
		printf("文件 %s 不存在，将创建新文件。\n", fileName);
		fp = fopen(fileName, "w");
		if (fp != NULL)
		{
			fprintf(fp, "服务站名称\t索赔编号\t索赔日期\t索赔金额\t底盘号\t审核人\n");
			fclose(fp);
		}
		return;
	}

	struct ClimeData tempData;
	char line[256];

	// 跳过标题行
	if (fgets(line, sizeof(line), fp) == NULL)
	{
		fclose(fp);
		return;
	}

	// 读取数据行
	while (fscanf(fp, "%29s\t%19s\t%19s\t%d\t%19s\t%29s",
				  tempData.ServiceStationName,
				  tempData.id,
				  tempData.ClaimTime,
				  &tempData.ClaimAmount,
				  tempData.CarVIN,
				  tempData.Reviewer) == 6)
	{
		insertNodeByHead(headNode, tempData);
	}

	fclose(fp);
	printf("成功从文件 %s 读取数据。\n", fileName);
}
// 打印索赔信息
void printList(struct Node *headNode)
{
	if (headNode == NULL || headNode->next == NULL)
	{
		printf("暂无索赔信息!\n");
		return;
	}

	struct Node *pMove = headNode->next;
	printf("\n=== 以下是现在储存的信息 ===\n");
	int count = 1;

	while (pMove != NULL)
	{
		printf("\n--- 记录 %d ---\n", count++);
		printf("服务站名称: %s\n", pMove->data.ServiceStationName);
		printf("索赔编号: %s\n", pMove->data.id);
		printf("索赔日期: %s\n", pMove->data.ClaimTime);
		printf("索赔金额: %d 元\n", pMove->data.ClaimAmount);
		printf("底盘号: %s\n", pMove->data.CarVIN);
		printf("审核人: %s\n", pMove->data.Reviewer);
		pMove = pMove->next;
	}
	printf("\n=== 共显示 %d 条记录 ===\n", count - 1);
}
// 用户交互界面
void keyDown()
{
	int userKey = 0;
	int choose = 0;
	int money = 0;
	struct ClimeData tempData; // 临时变量存储索赔信息
	struct Node *result = NULL;
	scanf("%d", &userKey);
	switch (userKey)
	{
	case 1:
		printf("----------读取课设要求----------\n");
		// system("E:\\coding things\vscode cpp\\课设要求.docx");
		system("pause");
		printf("成功查看\n");
		break;
	case 2:
		printf("----------查询索赔信息----------\n");
		printf("请输入索赔编号:\n");
		scanf("%s", tempData.id);
		result = searchByCarID(list, tempData.id);
		if (result == NULL)
			printf("未找到相关信息");
		else
		{
			printf("\n服务站名称: %s\n", result->data.ServiceStationName);
			printf("索赔编号: %s\n", result->data.id);
			printf("索赔日期: %s\n", result->data.ClaimTime);
			printf("索赔金额: %d\n", result->data.ClaimAmount);
			printf("底盘号: %s\n", result->data.CarVIN);
			printf("审核人: %s\n", result->data.Reviewer);
			printf("查询完毕\n");
		}
		break;
	case 3:
		printf("----------查询车辆历史索赔----------\n");
		printf("请输入车底盘号\n");
		scanf("%s", tempData.CarVIN);
		result = searchByCarVIN(list, tempData.CarVIN);
		if (result != NULL)
		{
			printf("查询成功以下是历史信息\n");
			searchAndPrintByCarVIN(list, tempData.CarVIN);
		}
		else
		{
			printf("请输入正确的编号\n");
		}
		break;
	case 4:
		printf("----------修改索赔信息----------\n");
		printf("请输入要修改信息的编号\n");
		scanf("%s", tempData.id);
		result = searchByCarID(list, tempData.id);
		if (result == NULL)
		{
			printf("请输入正确的索赔编号");
		}
		else
		{
			printf("请选择要修改的内容\n");
			printf("1.服务站名称||2.索赔日期||3.索赔金额||4.底盘号||5.审核人\n");
			printf("注意!如要修改编号，删除编号重新添加即可\n");
			printf("-----请输入数字-----\n");
			scanf("%d", &choose);
			if (choose > 0 && choose < 6)
			{
				switch (choose)
				{
				case 1:
					printf("请输入修改后的服务站名称");
					scanf("%s", tempData.ServiceStationName);
					strcpy(result->data.ServiceStationName, tempData.ServiceStationName);
					printf("修改成功！\n");
					break;
				case 2:
					printf("请输入修改后的索赔日期");
					scanf("%s", tempData.ClaimTime);
					if (checkTrueTime(tempData.ClaimTime) == 0)
					{
						printf("输入的日期不合法，请输入合法日期 例yyyy-mm-dd\n");
					}
					else
					{
						strcpy(result->data.ClaimTime, tempData.ClaimTime);
						printf("修改成功！\n");
					}
					break;
				case 3:
					printf("请输入修改后的索赔金额");
					scanf("%d", &tempData.ClaimAmount);
					result->data.ClaimAmount = tempData.ClaimAmount;
					printf("修改成功！\n");
					break;
				case 4:
					printf("请输入修改后的底盘号");
					scanf("%s", tempData.CarVIN);
					strcpy(result->data.CarVIN, tempData.CarVIN);
					printf("修改成功！\n");
					break;
				case 5:
					printf("请输入修改后的审核人名字");
					scanf("%s", tempData.Reviewer);
					strcpy(result->data.Reviewer, tempData.Reviewer);
					printf("修改成功！\n");
					break;
				}
			}
			else
			{
				printf("请输入正确数字\n");
			}
		}
		break;
	case 5:
		printf("----------删除索赔信息----------\n");
		printf("请输入要删除的索赔信息的编号：\n");
		scanf("%s", tempData.id);
		deleteNodeByClimeID(list, tempData.id);
		saveInfoFile("ClimeData.txt", list);
		break;
	case 6:
		printf("----------添加索赔信息----------\n");
		printf("请输入信息:\n");
		printf("服务站名称:");
		scanf("%s", tempData.ServiceStationName);
		printf("\n索赔编号: ");
		scanf("%s", tempData.id);
		result = checkRepeatID(list, tempData.id);
		if (result != NULL)
		{
			printf("ID重复,请重新输入正确ID\n");
			break;
		}
		printf("\n索赔日期: ");
		scanf("%s", tempData.ClaimTime);
		if (checkTrueTime(tempData.ClaimTime) == 0)
		{
			printf("\n输入的日期不合法，请输入合法日期 例yyyy-mm-dd\n");
			break;
		}
		printf("\n索赔金额: ");
		scanf("%d", &tempData.ClaimAmount);
		printf("\n底盘号: ");
		scanf("%s", tempData.CarVIN);
		printf("\n审核人: ");
		scanf("%s", tempData.Reviewer);
		printf("\n添加完毕");
		insertNodeByHead(list, tempData);
		saveInfoFile("ClimeData.txt", list);
		break;
	case 7:
		printf("----------查看统计信息----------\n");
		printf("选择方式统计金额：\n");
		printf("1.按照服务站统计2.按照审核人统计3.按照日期统计\n");
		scanf("%d", &choose);
		switch (choose)
		{
		case 1:
			printf("请输入服务站名\n");
			scanf("%s", tempData.ServiceStationName);
			money = statisticalAmountByName(list, tempData.ServiceStationName);
			printf("这是统计后的数字：%d\n", money);
			break;
		case 2:
			printf("请输入审核员名字\n");
			scanf("%s", tempData.Reviewer);
			money = statisticalAmountByReviewer(list, tempData.Reviewer);
			printf("这是统计后的数字：%d\n", money);
			break;
		case 3:
			printf("请输入日期 格式:yyyy-mm-dd\n");
			scanf("%s", tempData.ClaimTime);
			if (checkTrueTime(tempData.ClaimTime) == 0)
				printf("输入的日期不合法，请输入合法日期 例yyyy-mm-dd\n");
			else
			{
				money = statisticalAmountByTime(list, tempData.ClaimTime);
				printf("这是统计后的数字：%d\n", money);
			}
			break;
		}
		break;
	case 8:
		printf("----------退出该程序----------\n");
		saveInfoFile("ClimeData.txt", list);
		printf("【退出程序】\n");
		printf("【退出成功！】\n");
		system("pause");
		exit(0);
		break;
	default:
		printf("\terror!\n");
		printf("\t请输入数字 0~8\n");
		break;
	}
}
// 主函数
int main(void)
{
	welcome();

	// 初始化当前时间
	getNowTime();

	// 创建链表头
	list = createHead();
	if (list == NULL)
	{
		printf("程序初始化失败！\n");
		return -1;
	}

	// 读入数据
	readInfoFile("ClimeData.txt", list);

	while (1)
	{
		makeMenu();
		keyDown();
		system("pause");
		system("cls");
	}

	// 释放内存（虽然程序退出时系统会自动释放，但这是好习惯）
	freeList(list);

	return 0;
}

// 释放链表内存
void freeList(struct Node *headNode)
{
	if (headNode == NULL)
		return;

	struct Node *current = headNode;
	struct Node *next;

	while (current != NULL)
	{
		next = current->next;
		free(current);
		current = next;
	}
}
