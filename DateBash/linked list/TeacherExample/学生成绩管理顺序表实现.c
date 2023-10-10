#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXSIZE 1024
typedef struct student
{
	char cno[10];
	char cname[10];
	int score;
} elemtype;

typedef struct stuscorelist
{
	elemtype data[MAXSIZE];
	int last;
} StuScoreList;
StuScoreList *Init_StuScoreList();
int StuScoreList_Length(StuScoreList *L);
int Insert_StuScoreList(StuScoreList *L);
int Delete_StuScoreList(StuScoreList *L);
void ShowMenu();
void LoadInitData(StuScoreList *L);
void Print(StuScoreList *L);
void SearchByName(StuScoreList *L);
int JudgeCnoValid(StuScoreList *L, char cno[]);
int ModifyStuInfo(StuScoreList *L);
// 初始化顺序表
StuScoreList *Init_StuScoreList()
{
	StuScoreList *L;
	L = (StuScoreList *)malloc(sizeof(StuScoreList));
	if (L != NULL)
	{
		L->last = -1;
	}
	return L;
}

int StuScoreList_Length(StuScoreList *L)
{
	return L->last + 1;
}
// 在指定位置插入学生信息
int Insert_StuScoreList(StuScoreList *L)
{
	int i, j;
	elemtype x;
	if (L->last >= MAXSIZE - 1)
	{
		return 0;
	}
	Print(L);
	printf("输入要插入的位置：");
	scanf("%d", &i);
	if (i < 1 || i > L->last + 2)
	{
		return -1;
	}
	while (1)
	{
		printf("输入新学生的信息:\n");
		printf("学号:");
		fflush(stdin);
		gets(x.cno);
		if (JudgeCnoValid(L, x.cno) == 1)
		{
			printf("姓名:");
			fflush(stdin);
			gets(x.cname);
			printf("成绩:");
			scanf("%d", &x.score);
			break;
		}
		else
		{
			printf("学号重复，请重新输入！\n");
			system("PAUSE");
			continue;
		}
	}
	for (j = L->last; j >= i - 1; j--)
	{
		L->data[j + 1] = L->data[j];
	}
	L->data[i - 1] = x;
	L->last++;
	Print(L);
	return 1;
}
// 删除指定位置的学生信息
int Delete_StuScoreList(StuScoreList *L)
{
	int i, j;
	Print(L);
	printf("输入要删除学生的位置：");
	scanf("%d", &i);
	if (i < 1 || i > L->last + 1)
	{
		return 0;
	}
	else
	{
		for (j = i; j <= L->last; j++)
		{
			L->data[j - 1] = L->data[j];
		}
		L->last--;
	}
	Print(L);
	return 1;
}

// 系统功能菜单
void ShowMenu()
{
	system("CLS");
	printf("欢迎使用学生成绩管理系统\n");
	printf("1.新增学生信息\n");
	printf("2.删除学生信息\n");
	printf("3.查找学生信息\n");
	printf("4.修改学生信息\n");
	printf("5.显示学生信息\n");
	printf("0.退出系统\n");
	printf("请输入你的选择:");
}
// 从文件导入原始基础数据
void LoadInitData(StuScoreList *L)
{
	FILE *fp;
	fp = fopen("D:\\score.txt", "r+");
	int i;
	for (i = 0; i < 10; i++)
	{
		fscanf(fp, "%s%s%d", L->data[i].cno, L->data[i].cname, &L->data[i].score);
		L->last++;
	}
	fclose(fp);
}

// 学生信息输出
void Print(StuScoreList *L)
{
	int i;
	if (L->last == -1)
	{
		return;
	}
	printf("%10s%10s%10s\n", "学号", "姓名", "成绩"); // 输出表头
	for (i = 0; i <= L->last; i++)
	{

		printf("--------------------------------\n");
		printf("%10s%10s%10d\n", L->data[i].cno, L->data[i].cname, L->data[i].score);
	}
	printf("--------------------------------\n");
}
// 按名字查找学生
void SearchByName(StuScoreList *L)
{
	int i;
	char name[10];
	if (L->last == -1)
	{
		printf("学生信息为空，查找失败！\n");
		return;
	}
	printf("请输入要查找学生的名字:");
	fflush(stdin);
	gets(name);
	printf("%10s%10s%10s\n", "学号", "姓名", "成绩"); // 输出表头
	for (i = 0; i <= L->last; i++)
	{
		if (strstr(L->data[i].cname, name) != 0)
		{
			printf("--------------------------------\n");
			printf("%10s%10s%10d\n", L->data[i].cno, L->data[i].cname, L->data[i].score);
		}
	}
	printf("--------------------------------\n");
}
// 学号合法性判断（学号的唯一性）
int JudgeCnoValid(StuScoreList *L, char cno[])
{
	int i;
	for (i = 0; i <= L->last; i++)
	{
		if (strcmp(L->data[i].cno, cno) == 0)
		{
			return 0;
		}
	}
	return 1;
}
// 修改功能菜单
void ModifyShowMenu()
{
	// system("CLS");
	printf("1.修改学号\n");
	printf("2.修改姓名\n");
	printf("3.修改成绩\n");
	printf("0.退出系统\n");
	printf("请输入你的选择:");
}

// 学生信息修改
int ModifyStuInfo(StuScoreList *L)
{
	char cno[10];
	int i, pos = -1, choice;
	if (StuScoreList_Length(L) == 0)
	{
		printf("学生成绩信息表为空，修改失败!\n");
		return 0;
	}
	else
	{
		// 先找到要修改的学生信息
		printf("请输入要查找学生的学号:");
		fflush(stdin);
		gets(cno);
		// 开始判断现有学生信息表中是否存在该学号
		printf("%10s%10s%10s\n", "学号", "姓名", "成绩"); // 输出表头
		for (i = 0; i <= L->last; i++)
		{
			if (strcmp(L->data[i].cno, cno) == 0)
			{
				printf("--------------------------------\n");
				printf("%10s%10s%10d\n", L->data[i].cno, L->data[i].cname, L->data[i].score);
				pos = i;
				break;
			}
		}
		printf("--------------------------------\n");
		if (pos == -1)
		{
			printf("该学生不存在，修改失败！\n");
			return 0;
		}
		else
		{ // 完成对该生信息的修改
			ModifyShowMenu();
			scanf("%d", &choice);
			switch (choice)
			{
			case 1:
				while (1)
				{
					printf("请输入新的学号:");
					fflush(stdin);
					gets(cno);
					if (JudgeCnoValid(L, cno) == 1)
					{ // 学号不重复，可以修改。
						strcpy(L->data[pos].cno, cno);
						break;
					}
					else
					{
						continue;
					}
				}
				break;
			case 2:
				printf("请输入新的姓名:");
				fflush(stdin);
				gets(L->data[pos].cname);
				break;
			case 3:
				printf("请输入新的成绩:");
				fflush(stdin);
				scanf("%d", &L->data[pos].score);
				break;
			}
			printf("修改成功！\n");
			return 1;
		}
	}
}

int main()
{
	StuScoreList *StuList;
	int choice, pos;
	StuList = Init_StuScoreList();
	LoadInitData(StuList);
	while (1)
	{
		// 输出功能菜单
		ShowMenu();
		scanf("%d", &choice);
		switch (choice)
		{
		case 1:
			Insert_StuScoreList(StuList);
			system("PAUSE");
			break;
		case 2:
			Delete_StuScoreList(StuList);
			system("PAUSE");
			break;
		case 3:
			SearchByName(StuList);
			system("PAUSE");
			break;
		case 4:
			ModifyStuInfo(StuList);
			system("PAUSE");
			break;
		case 5:
			Print(StuList);
			system("PAUSE");
			break;
		case 0:
			printf("Bye bye!");
			return 0;
		}
	}
	return 0;
}
