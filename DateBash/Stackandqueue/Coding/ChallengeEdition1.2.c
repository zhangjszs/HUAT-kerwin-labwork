/*
 * @Description: 
 * @Version: 2.0
 * @Autor: kerwinz
 * @Date: 2023-11-14 10:38:37
 * @LastEditors: kerwinz
 * @LastEditTime: 2023-11-28 09:10:18
 */
/*
 从键盘上任意输入一个算术表达式，编写程序，计算表达式的值。
例如：键盘输入12+25*4,，自动计算出结果为112。
提示：利用堆栈来完成该问题的求解，设计两个栈，一个处理运算数，另一个处理运算符。
这是完整版本
*/
#include<stdio.h>
#include<string.h>

#define MaxSize 10000 
 
typedef struct CharStack				//字符栈 
{
	char data[MaxSize];
	int top;
}cStack;
 
typedef struct DoubleStack				//数据栈 
{
	double data[MaxSize];
	int top;
}dStack;
 
int Isop(char );			  		//当前扫描元素优先级 
int Inop(char );					//栈顶元素优先级 
 
void Initc(cStack *);				//初始化字符栈 
int Pushc(cStack *,char);			//字符栈压栈 
char Gettopc(cStack *);				//返回栈顶元素 
char Popc(cStack *);				//出栈 
 
void Initd(dStack *);				//初始化数据栈 
int Pushd(dStack *,double);			//数据压栈 
double Popd(dStack *);				//出栈 
 
void Trans(char*s1,char*s2);	
 
double Calculate(char *s2);			//后缀表达式求值 
 
 
 
 

int main()
{
	char s1[MaxSize];			//用于存储前缀表达式 
	char s2[MaxSize];			//用于存储转换后的表达式 
	printf("请输入表达式:");
	scanf("%s",s1);
	
	Trans(s1,s2);				//处理字符串，并转化为后缀表达式,存放在s2中 
	
	printf("计算结果为: %f",Calculate(s2));		//后缀表达式求值 
	
		
	return 0;
}
 
//初始化 
void Initc(cStack *s1)
{
	s1->top=-1;
}
 
//字符栈压栈 
int Pushc(cStack *c1,char op)
{
 	if(c1->top<MaxSize)
 	{
 		c1->data[++c1->top]=op;
 		return 1;
	}
	else 
    {
        printf("栈满");
        return 0;
    }
}
 
//获取栈顶元素 
char Gettopc(cStack *c1)
{
	return c1->data[c1->top];
}
 
//字符栈出栈 
char Popc(cStack *c1)
{
	return c1->data[c1->top--];
}
 
//初始化数据栈 
void Initd(dStack *d1)
{
	d1->top=-1;
}
 
//数据栈压栈 
int Pushd(dStack *d1,double data)
{
	if(d1->top<MaxSize)
	{
		d1->data[++d1->top]=data;
		return 1;
	}
	else return 0;
}
 
//数据栈出栈 
double Popd(dStack *d1)
{
	return d1->data[d1->top--];
}
 
int Isop(char op)		//当前扫描运算符优先级
{
	switch(op)
	{
		case '(': 
			return 6;
		case '+': 
		case '-':
			 return 2;
		case '*': 
		case '/':
			 return 4;
        default:
        {
            printf("输入错误");
            return -1;
        }
	}
}

int Inop(char op)		//栈顶元素优先级
{
	switch(op)
	{
		case '(':
			 return 1;
		case '+':
		case '-':
			 return 3;
		case '*':
		case '/':
			 return 5;
        default:
        {
            printf("输入错误");
            return -1;
        }
	}
}
 
void Trans(char *s1,char *s2)
{
	int i=0;
	int j=0;
	int flag1=-1;				//flag1为0表示上次输出为数字，flag1为1表示上次输出为字符
	int flag2=-1; 				//flag2为0表示上次扫描为数字，flag为1表示上次扫描为运算符，用于区分数字后加空格
	cStack st1;					//暂放运算符 
	Initc(&st1);
	
	while(s1[i]!='\0')			//处理负数 
	{
		if(s1[0]=='-')			//第一位数字为负数时 
		{
			j=strlen(s1);
			while(j>0)
			{
				s1[j+5]=s1[j];
				j--;
			}
			s1[j++]='(';
			s1[j++]='0';
			s1[j++]='-';
			s1[j++]='1';
			s1[j++]=')';
			s1[j]='*';
			
		}
		if(s1[i]=='('&&s1[i+1]=='-')	//非第一位负数时 
		{
			j=strlen(s1);
			while(j>i+1)
			{
				s1[j+5]=s1[j];
				j--;
			}
			s1[j++]='(';
			s1[j++]='0';
			s1[j++]='-';
			s1[j++]='1';
			s1[j++]=')';
			s1[j]='*';
			i=i+5;
		}
		i++;
	}
	
	i=0;
	j=0;
	while(s1[i]!='\0')
	{
		if(flag1==0&&flag2==1)		//若上次的输出为数字，上次循环扫描为字符，则表示该数字串结束，则在数字后加空格区分 
		{
			s2[j++]=' ';
			flag1=1; 
		}
		if((s1[i]>='0'&&s1[i]<='9')||(s1[i]=='.'))
		{
			s2[j++]=s1[i];
			flag2=0;
			flag1=0;
		}
		else if(s1[i]=='+'||s1[i]=='-'||s1[i]=='*'||s1[i]=='/'||s1[i]=='(')
		{
			flag2=1;
			if(st1.top<0||Isop(s1[i])>Inop(Gettopc(&st1)))
			{
				Pushc(&st1,s1[i]);
			}
			else
			{
				while(st1.top>=0&&Isop(s1[i])<Inop(Gettopc(&st1)))		//当前扫描字符优先级不断与栈顶字符优先级比较，当前字符小于栈顶字符时退栈并输出 
				{
						s2[j++]=Popc(&st1); 
						flag1=1;
				}
				if(st1.top<0||Isop(s1[i])>Inop(Gettopc(&st1)))			//当前字符优先级大于栈顶优先级或栈空时当前字符压入字符栈内 
				{
					Pushc(&st1,s1[i]);
				}
			}
		}
		else if(s1[i]==')')				 
		{
			flag2=1;
			if(Gettopc(&st1)!='(')		//若括号仅包含数字则没有输出运算符 
			{
				flag1=1;
			}
			while(Gettopc(&st1)!='(')
			{
				s2[j++]=Popc(&st1);
			}
			Popc(&st1);		//将'('出栈 
		}
		i++;
	}
	while(st1.top>=0)		//将栈内剩余的运算符依次退栈输出 
	{
		s2[j++]=Popc(&st1);
	}
	s2[j]='\0';
}
 
//表达式求值 
double Calculate(char *s1)
{
	int i=0;
	int flag;				//char类型转换为double类型数据标记 
	double data1,data2;
	double sum;
	dStack ds1;
	Initd(&ds1);
	while(s1[i]!='\0')
	{
		if(s1[i]=='+'||s1[i]=='-'||s1[i]=='*'||s1[i]=='/')			//若为运算符获取栈顶两个元素进行计算 
		{
			data1=Popd(&ds1);
			data2=Popd(&ds1);
			if(s1[i]=='+') Pushd(&ds1,data2+data1);
			else if(s1[i]=='-') Pushd(&ds1,data2-data1);
			else if(s1[i]=='*') Pushd(&ds1,data2*data1);
			else if(s1[i]=='/') Pushd(&ds1,data2/data1);
		}
		else							//为数据时转化为double类型压栈 
		{
			flag=0;					//初始化为0为整数部分标记，1为小数部分标记 
			sum=0;
			double divider=1;
			while(s1[i]!=' '&&s1[i]!='+'&&s1[i]!='-'&&s1[i]!='*'&&s1[i]!='/')
			{
				if(s1[i]=='.')		//若有小数点，进入小数转化模式 
				{
					flag=1;
					i++;
					continue;
				}
				if(flag==0)
				{
					sum=sum*10+(double)(s1[i]-'0');
				}
				else
				{
					divider=divider*10;
					sum=sum+((double)(s1[i]-'0'))/divider;
				}
				i++;
			}
			if(s1[i]=='+'||s1[i]=='-'||s1[i]=='*'||s1[i]=='/') i--;	//转化成功一个数据，若下个字符为运算符，则i--，回到当前运算的数据位置 
			Pushd(&ds1,sum);
		}
		i++;		//i++准备下一个字符的转换 
	}
	 return Popd(&ds1);
}