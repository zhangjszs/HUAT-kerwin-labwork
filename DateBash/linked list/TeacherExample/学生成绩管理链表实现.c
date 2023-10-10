#include<stdio.h>
#include<stdlib.h>
#include<string.h> 
#define MAXSIZE 1024
typedef struct student
{
	char cno[10];
	char cname[10];
	int  score; 
}elemtype;

typedef struct node
{
	elemtype data;
	struct node *next;
}LinkList;

LinkList *Create_LinkListR();
int  Print_LinkList(LinkList *head);
int  LinkList_Length(LinkList *head);
void InsertBypos(LinkList *head);
void Insert(LinkList *head);
void DeleteBypos(LinkList *head);
void ShowMenu();
int SearchByName(LinkList *head);

//尾插法建立单链表 (从文件导入原始数据)
LinkList *Create_LinkListR()
{
	elemtype ix;
	int i;
	LinkList *head,*p,*tail;
	FILE *fp;
	fp=fopen("D:\\score.txt","r+");
	head=(LinkList *)malloc(sizeof(LinkList));
	if(head==NULL)
	{
		return head;
	}
	head->next=NULL;
	tail=head;
	for(i=0;i<10;i++)
	{
		p=(LinkList *)malloc(sizeof(LinkList));
		if(p==NULL)
		{
			return head;
		}
		fscanf(fp,"%s%s%d",p->data.cno,p->data.cname,&p->data.score);
		tail->next=p;
		tail=p;
		tail->next=NULL;
	}
	fclose(fp);
	return head;	
}

//信息输出函数
int Print_LinkList(LinkList *head)
{
	LinkList *p;
	p=head->next;
	if(p==NULL)
	{
		return 0;
	}
	printf("%10s%10s%10s\n","学号","姓名","成绩");//输出表头
	while(p!=NULL)
	{
			
			printf("-----------------------------------\n"); 
			printf("%10s%10s%10d\n",p->data.cno,p->data.cname,p->data.score);
			p=p->next; 
	}
	printf("-----------------------------------\n");
} 

//链表长度的计算 
int  LinkList_Length(LinkList *head)
{
	LinkList *p=head;
	int j=0;
	while(p->next!=NULL)
	{
		p=p->next;
		j++;
	}
	return j;
} 

//插入一个新学生的信息（新增的信息位置自由指定） 
void InsertBypos(LinkList *head)
{
	int pos,count=0;//pos表示插入学生的位置
	LinkList *s,*p;
	char cno[10];
	Print_LinkList(head);
	printf("请输入新增学生的信息:\n");
	printf("位置:");
	scanf("%d",&pos);
	s=(LinkList *)malloc(sizeof(LinkList));
	s->next=NULL; 
	while(1)
	{
		printf("学号:");
		fflush(stdin);
		gets(cno);
		if(JudgeCnoValid(head,cno)==1)
		{
			strcpy(s->data.cno,cno);
			break;
		}
		else
		{
			printf("学号重复，请重新输入!\n");
		}
	}	
	printf("姓名:");
	fflush(stdin);
	gets(s->data.cname);
	printf("成绩:");
	scanf("%d",&s->data.score);
	//通过循环，先找到pos-1这个位置
	p=head;
	while(p->next!=NULL)
	{
		if(count==pos-1)
		{
			break;
		}
		count++;
		p=p->next;
	}
	//将新增的结点，插入到原来的链表中去
	s->next=p->next;
	p->next=s; 
	Print_LinkList(head);
} 

//插入一个新学生的信息（新增的信息插入原链表的最后） 
void Insert(LinkList *head)
{
	LinkList *s,*p;
	char cno[10]; 
	p=head->next;
	Print_LinkList(head);
	while(p->next!=NULL)//循环的作用是查找链表中最后一个结点 
	{
		p=p->next;
	}
	s=(LinkList *)malloc(sizeof(LinkList));
	s->next=NULL;
	printf("请输入新增学生的信息:\n");
	while(1)
	{
		printf("学号:");
		fflush(stdin);
		gets(cno);
		if(JudgeCnoValid(head,cno)==1)
		{
			strcpy(s->data.cno,cno);
			break;
		}
		else
		{
			printf("学号重复，请重新输入!\n");
		}
	}	
	printf("姓名:");
	fflush(stdin);
	gets(s->data.cname);
	printf("成绩:");
	scanf("%d",&s->data.score);
	p->next=s;
	Print_LinkList(head);
}

//删除指定位置的结点
void DeleteBypos(LinkList *head)
{
	int pos,count=0;//表示插入学生的位置
	LinkList *r,*p;
	Print_LinkList(head);
	printf("请输入需要删除学生的位置:\n");
	scanf("%d",&pos);
	//通过循环，先找到pos-1这个位置
	p=head;
	while(p->next!=NULL)
	{
		if(count==pos-1)
		{
			break;
		}
		count++;
		p=p->next;//p指向的是第pos-1个位置 
	}
	r=p->next;//r就是指向的是第pos个位置
	p->next=r->next;
	Print_LinkList(head);
	free(r);	
} 

//按名字查找学生 
int SearchByName(LinkList *head)
{
	char name[10];
	int flag=0;
	LinkList *p=head->next;;
	if(p==NULL)
	{
		printf("学生信息为空，查找失败！\n");
		return 0;
	}
	printf("请输入要查找学生的名字:");
	fflush(stdin);
	gets(name);
	printf("%10s%10s%10s\n","学号","姓名","成绩");//输出表头
	while(p!=NULL)
	{
			if(strcmp(p->data.cname,name)==0)
			{
				flag=1;
				printf("--------------------------------\n"); 
				printf("%10s%10s%10d\n",p->data.cno,p->data.cname,p->data.score);				
			}
			p=p->next;			
	}
	if(flag==0)
	{
		printf("不存在该学生，查找失败！\n");
		return 0;
	}
	printf("--------------------------------\n");
	return 1;
}

//学号合法性判断（学号的唯一性） 
int JudgeCnoValid(LinkList *head,char cno[]) 
{
	LinkList *p=head->next;
	while(p!=NULL)
	{
		if(strcmp(p->data.cno,cno)==0)
		{
			return 0;
		}
		p=p->next;
	}
	return 1;
}

//系统功能菜单 
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

//修改功能菜单 
void ModifyShowMenu()
{
	//system("CLS");
	printf("1.修改学号\n");
	printf("2.修改姓名\n");
	printf("3.修改成绩\n");
	printf("0.退出系统\n");
	printf("请输入你的选择:");
}

//学生信息修改
int main()
{
	LinkList *head;
	int choice,pos;
	head=Create_LinkListR();//头插法建立链表，原始数据从文件中导入。 
	while(1)
	{
		//输出功能菜单
		ShowMenu();
		scanf("%d",&choice);
		switch(choice)
		{
			case 1: InsertBypos(head);system("PAUSE");break;
			case 2: DeleteBypos(head);system("PAUSE");break;
			case 3: SearchByName(head);system("PAUSE");break;
			case 4: //ModifyStuInfo(StuList);system("PAUSE");break;
			case 5: Print_LinkList(head);system("PAUSE");break;
			case 0: printf("Bye bye!");return 0;
		} 
	}
	return 0;
}


