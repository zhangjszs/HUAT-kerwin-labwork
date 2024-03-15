#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <process.h>
#include <conio.h>
#include <time.h>
#define MAXNAME 30
#define MAXDAY 20
#define MAXID 20
typedef long long ll;
struct Node *list = NULL; // 使用全局链表
int Nowyear, Nowmonth, Nowday;
int monthday[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
void welcome()
{
	system("color F0");
	printf("\n\n\n\n\t\t ********************************************************************\n");
	printf("\t\t ********************************************************************\n");
	printf("\t\t ********************************************************************\n");
	printf("\t\t **  \t\t\t\t\t\t\t\t   **\n");
	printf("\t\t **  \t\t\t\t\t\t\t\t   **\n");
	printf("\t\t **   欢   迎   使   用   汽   车   索   赔   管   理   系   统 !  ** \n");
	printf("\t\t **  \t\t\t\t\t\t\t\t   **\n");
	printf("\t\t **  \t\t\t\t\t\t\t\t   **\n");
	printf("\t\t **  \t\t\t\t\t\t\t\t   **\n");
	printf("\t\t **  \t\t\t\t\t\t\t\t   **\n");
	printf("\t\t **  \t\t\t\t\t\t\t\t   **\n");
	printf("\t\t **   制   作   人：章崇文       指   导   教   ；史旅华 齐心      **\n");
	printf("\t\t **   学校 湖北汽车工业学院      班级 计算机 222                   **\n");
	printf("\t\t ********************************************************************\n");
	printf("\t\t *************   请   按   ENTER   键   进   入   系   统************\n");
	printf("\t\t ********************************************************************\n");
	fflush(stdin);
	getchar();
	system("cls");
	return;
}
void makeMenu() // 菜单界面
{
	printf("\n\n\t\t--------------------成 功 进 入 系 统 !--------------------\n");
	printf("\t\t--------------------【以 下 是 菜 单：】--------------------\n");
	printf("\t\t---------------        1.0读取课设要求       ---------------\n");
	printf("\t\t---------------        2.0查询索赔信息       ---------------\n");
	printf("\t\t---------------        3.0查询车辆索赔历史   ---------------\n");
	printf("\t\t---------------        4.0修改索赔信息       ---------------\n");
	printf("\t\t---------------        5.0删除索赔信息       ---------------\n");
	printf("\t\t---------------        6.0添加索赔信息       ---------------\n");
	printf("\t\t---------------        7.0查看统计信息       ---------------\n");
	printf("\t\t---------------        8.0退出该程序         ---------------\n");
	printf("\t\t------------------------------------------------------------\n");
	printf("\t\t----如有问题到 https://www.cnblogs.com/jszs0013/进行留言----\n");
	return;
}
// 索赔信息的设定
struct ClimeData
{
	char ServiceStationName[MAXNAME]; // 服务站名称
	char id[MAXID];					  // 索赔编号
	int ClaimAmount;				  // 索赔金额
	char ClaimTime[MAXDAY];			  // 索赔日期
	char CarVIN[MAXID];				  // 底盘号
	char Reviewer[MAXNAME];			  // 审核人
};
// 主要数据链表的设定
struct Node
{
	struct ClimeData data;
	struct Node *next;
};
struct Node *createHead()
{
	struct Node *headNode = (struct Node *)malloc(sizeof(struct Node)); // 动态内存申请
	headNode->next = NULL;
	return headNode;
};
// 创建节点为数据做准备
struct Node *createNode(struct ClimeData data)
{
	struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
	newNode->data = data;
	newNode->next = NULL;
	return newNode;
}
// 插入数据 使用表头法插入
void insertNodeBYHead(struct Node *headNode, struct ClimeData data)
{
	struct Node *newNode = createNode(data);
	newNode->next = headNode->next;
	headNode->next = newNode;
}
// 插入数据 使用表尾法
void insertNodeBYTail(struct Node *headNode, struct ClimeData data)
{
	struct Node *pMove = headNode;
	while (pMove != NULL)
	{
		pMove = pMove->next;
	}
	struct Node *newNode = createNode(data);
	pMove->next = newNode;
}
// 指定位置删除，给定索赔编号，删除索赔信息
void deleteNodeBYClimeID(struct Node *headNode, char *DataId)
{
	struct Node *posLeftNode = NULL;
	struct Node *posNode = headNode->next;
	while (posNode != NULL && strcmp(posNode->data.id, DataId)) // 比较id
	{
		posLeftNode = posNode;
		posNode = posLeftNode->next;
	}
	// 讨论查找结果
	if (posNode == NULL)
		return;
	else
	{
		printf("删除成功！\n");
		posLeftNode->next = posNode->next;
		free(posNode);
		posNode = NULL;
	}
}
// 用底盘号查找
struct Node *searchByCarVIN(struct Node *headNode, char *CarVIN)
{
	struct Node *posNode = headNode->next;
	while (posNode != NULL && strcmp(posNode->data.CarVIN, CarVIN))
	{
		posNode = posNode->next;
	}
	return posNode;
}
// 通过底盘号查历史
void searchAndprintfBYCarVIN(struct Node *headNode, char *CarVIN)
{

	struct Node *posNode = headNode->next;
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
		}
	}
}
// 用编号查找
struct Node *searchByCarID(struct Node *headNode, char *ID)
{
	struct Node *posNode = headNode->next;
	while (posNode != NULL && strcmp(posNode->data.id, ID))
	{
		posNode = posNode->next;
	}
	return posNode;
}
struct Node *checkRepeatID(struct Node *headNode, char *ID) // 判断编号是否重复
{
	struct Node *posNode = headNode->next;
	while (posNode != NULL && strcmp(posNode->data.id, ID))
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
void Nowtime() // 通过函数得到当前时间
{
	time_t timep;
	time(&timep);
	struct tm *p;
	p = gmtime(&timep);
	Nowyear = (1900 + p->tm_year);
	Nowmonth = 1 + p->tm_mon;
	Nowday = p->tm_mday;
	// printf("%d-%d-%d",Nowyear,Nowmonth,Nowday);
}
int checkTrueTime(char *time) // 判断时间合法性
{
	int flagyear = 0, flagmonth = 0, flagday = 0, trueflag = 0;
	char ID[15];
	strcpy(ID, time);
	printf("%s", ID);
	char syear[10], smonth[10], sday[10];
	substr(syear, ID, 0, 4);
	substr(smonth, ID, 5, 2);
	substr(sday, ID, 8, 2);
	int putyear = atoi(syear);
	int putmonth = atoi(smonth);
	int putday = atoi(sday);
	if (IsLeapYear(putyear))
		monthday[2] = 29;
	if (putyear <= Nowyear && putyear > 0)
	{
		flagyear = 1;
		if (putmonth <= Nowmonth && putmonth > 0)
			flagmonth = 1;
		if (putday < monthday[putmonth] && putday > 0)
			flagday = 1;
	}
	if (flagday && flagmonth && flagyear)
		trueflag = 1;
	/*else
	{
		printf("输入的日期不合法，请输入合法日期 例yyyy-mm-dd\n");
	}*/
	return trueflag;
}
// 通过服务站统计金额
int StatisticalAmountByName(struct Node *headNode, char *name)
{
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
int StatisticalAmountByReviewer(struct Node *headNode, char *name)
{
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
int StatisticalAmountByTime(struct Node *headNode, char *Time)
{
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
// 写入文件
void saveInfoFile(const char *fileName, struct Node *headNode)
{
	FILE *fp = fopen(fileName, "w+");
	struct Node *pMove = headNode->next;
	fprintf(fp, "服务站名称\t索赔编号\t索赔日期\t索赔金额\t底盘号\t审核人\t\n");
	while (pMove != NULL)
	{
		fprintf(fp, "%s\t%s\t%s\t%d\t%s\t%s\t\n", pMove->data.ServiceStationName, pMove->data.id, pMove->data.ClaimTime, pMove->data.ClaimAmount,
				pMove->data.CarVIN, pMove->data.Reviewer);
		pMove = pMove->next;
	}
	fclose(fp);
}
// 文件读操作
void readInfoFile(const char *fileName, struct Node *headNode)
{
	FILE *fp = fopen(fileName, "r");
	if (fp == NULL) // 文件不纯在就创建文件
	{
		fp = fopen(fileName, "w+");
	}
	struct ClimeData tempData;
	fscanf(fp, "%*[^\n]");
	while (
		(fscanf(fp, "%s\t", tempData.ServiceStationName) != EOF) && (fscanf(fp, "%s\t", tempData.id) != EOF) &&
		(fscanf(fp, "%s\t", tempData.ClaimTime) != EOF) && (fscanf(fp, "%d", &tempData.ClaimAmount)) &&
		(fscanf(fp, "%s\t", tempData.CarVIN) != EOF) && (fscanf(fp, "%s\n", tempData.Reviewer)))
	{
		insertNodeBYHead(list, tempData);
	}
	fclose(fp);
}
// 打印索赔信息
void printfList(struct Node *headNode)
{
	struct Node *pMove = headNode->next;
	printf("以下是现在储存的信息");
	while (pMove != NULL)
	{
		printf("\n服务站名称: %s\n", pMove->data.ServiceStationName);
		printf("索赔编号: %s\n", pMove->data.id);
		printf("索赔日期: %s\n", pMove->data.ClaimTime);
		printf("索赔金额: %d\n", pMove->data.ClaimAmount);
		printf("底盘号: %s\n", pMove->data.CarVIN);
		printf("审核人: %s\n", pMove->data.Reviewer);
	}
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
		printf("----------查询车辆索赔历史----------\n");
		printf("请输入车底盘号\n");
		scanf("%s", tempData.CarVIN);
		result = searchByCarVIN(list, tempData.CarVIN);
		if (result != NULL)
		{
			printf("查询成功以下是历史信息\n");
			searchAndprintfBYCarVIN(list, tempData.CarVIN);
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
		deleteNodeBYClimeID(list, tempData.id);
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
		insertNodeBYHead(list, tempData);
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
			money = StatisticalAmountByName(list, tempData.ServiceStationName);
			printf("这是统计后的数字：%d\n", money);
			break;
		case 2:
			printf("请输入审核员名字\n");
			scanf("%s", tempData.Reviewer);
			money = StatisticalAmountByReviewer(list, tempData.Reviewer);
			printf("这是统计后的数字：%d\n", money);
			break;
		case 3:
			printf("请输入日期 格式:yyyy-mm-dd\n");
			scanf("%s", tempData.ClaimTime);
			if (checkTrueTime(tempData.ClaimTime) == 0)
				printf("输入的日期不合法，请输入合法日期 例yyyy-mm-dd\n");
			else
			{
				money = StatisticalAmountByTime(list, tempData.ClaimTime);
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
int main()
{
	welcome();
	list = createHead();
	readInfoFile("ClimeData.txt", list); // 读入数据

	while (1)
	{
		makeMenu();
		keyDown();
		system("pause");
		system("cls");
	}
	system("pause");
	return 0;
}
