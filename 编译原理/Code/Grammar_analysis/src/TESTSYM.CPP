#include "basedata.h"
#include "symbol.h"
#include "ExpressionCalculator.hpp"
#include <fstream>
#include <exception>
#include <iostream>
// #include <conio.h>
extern char *WORD[WORDLEN];
extern int WSYM[WORDLEN];
extern char* SNAME[SYMBOLNUM];
extern SYMBOL sym;//last readed word type;
extern char   token[MAXIDLEN+1];//last readed word
extern int num;//last readed num;
extern char ch;//last readed char;
extern int col,row;
extern FILE *fd;
FILE *fout;
void Init(void);
void Quit(void);
//======================================
void Init(void)
{
  char temp[30];
  //printf("\nPlease input your file name:");
  //gets(temp);
  if ((fd = fopen("b.txt","rt"))
== NULL)
  {
   fprintf(stderr, "Cannot open input file %s.\n",temp);
   getchar();
   return ;
  }//将fd指针指向待分析源文件
  if ((fout = fopen("mydata.dat", "wt"))
== NULL)
  {
   fprintf(stderr, "Cannot open input file.\n");
   getchar();
   return ;
  }//将fout指向文件mydata.dat
}
void Quit(void)
{
  fclose(fd);
  fclose(fout);
}


int main()
{
  int flag;
  Init();

  fprintf(fout,"\n     TOKEN       SYM         NUM");
  printf("\n     TOKEN           SYM       NUM\n");
  do{
	
	flag=Getsym();
	if(flag)
	 {
	  fprintf(fout,"\n%10s    %10s        %d",token,SNAME[sym],num);
	  printf("\n%10s    %10s        %d",token,SNAME[sym],num);
	 }
	else if(ch!=EOF)
	 {
	  fprintf(fout,"\n%10s",token);
	  printf("\n%10s",token);
	  Errorsym();
	 }
	}while(ch!=EOF);//反复调用Getsym()识别单词，将输出结果存入fout中
   std::ifstream inFile("b.txt"); // 打开文件
    if (!inFile.is_open()) {
        std::cerr << "无法打开文件 b.txt" << std::endl;
        return 1; // 文件打开失败，返回错误代码
    }

    std::string expression;
    std::getline(inFile, expression); // 读取文件中的整行内容作为表达式

    try {
        // 调用 evaluateExpression 函数计算表达式
        double result = evaluateExpression(expression);
        
        // 输出结果
        std::cout << std::endl << "结果是: " << result << std::endl;
    } catch (const std::runtime_error& e) {
        // 如果发生异常，输出错误信息
        std::cerr << "发生错误: " << e.what() << std::endl;
        inFile.close(); // 关闭文件
        return 1; // 返回非零值表示程序异常退出
    }

    inFile.close(); // 关闭文件
  Quit();
  return 0;
}