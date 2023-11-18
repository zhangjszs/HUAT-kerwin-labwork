// 在顺序存储的基础上进行字符串长度计算，字符串的连接，字符串的替换，求子串运算。
#include <stdio.h>
#include <stdlib.h>
#define MAXSIZE 256 // 定义串允许的最大字符个数
typedef struct
{
    char string[MAXSIZE]; // MAXSIZE为串的最大长度
    int len;              // 串的实际长度
} SSTRING;                // 串的顺序存储结构表示

int str_len(SSTRING *s) // 串的长度计算函数
{
    return s->len;
}

void str_input(SSTRING *s) // 串的输入函数
{
    int i = 0;
    char ch;
    s->len = 0;
    while ((ch = getchar()) != '\n') // 通过循环，从键盘输入一串字符串
    {
        s->string[i] = ch;
        i++;
        s->len++;
    }
    s->string[i] = '\0';
}

SSTRING *str_rep(SSTRING *s1, SSTRING *s2, int pos)
{ // 将字符串S1从pos位置处替换S2串
    int i = 0, j;
    for (j = pos - 1; s1->string[j] != '\0'; j++) // 从替换的起始位置开始
        if (s2->string[i] != '\0')
        {
            s1->string[j] = s2->string[i]; // 进行替换
            i++;
        }
        else
        {
            break;
        }
    return s1;
}

SSTRING *str_con(SSTRING *s1, SSTRING *s2)
{
    int i = s1->len, j = 0, k;
    for (k = 0; k < str_len(s2); k++)
    {
        s1->string[i] = s2->string[j]; // 通过循环，将S2串的元素逐一复制到S1串的后面，
        i++;                           // 合并成一个串。
        j++;
    }
    s1->len = i;
    s1->string[i] = '\0';
    return s1;
}

SSTRING *str_sub(SSTRING *sub, SSTRING *s, int pos, int len)
{
    int i = pos - 1, j = 0, k;
    for (k = 1; k <= len; k++)
    {
        sub->string[j] = s->string[i]; // 逐一取出S串中从pos开始的元素，并复制到sub串中
        i++;                           // 每复制完一个元素，sub串长度加1。
        j++;
    }
    sub->len = j;
    sub->string[j] = '\0';
    return sub;
}

void str_print(SSTRING *s)
{
    int i;
    for (i = 0; i < s->len; i++)
        printf("%c", s->string[i]); // 依次将串中的字符逐一输出到屏幕显示。
    printf("\n");  
}

void showmenu()
{ // 显示菜单
    printf("    欢迎使用串操作小软件\n");
    printf("\t1、求串的长度\n");
    printf("\t2、串的替换\n");
    printf("\t3、串的连接\n");
    printf("\t4、求子串\n");
    printf("\t5、退出程序\n");
}

int main()
{
    int position, length, no;
    SSTRING str, str1, str2;
    str.len = 0;
    str1.len = 0, str2.len = 0;
    while (1)
    {
        showmenu();
        printf("    请输入你的选择：");
        scanf("%d", &no);
        switch (no)
        {
        case 1:
            printf("请输入一串字符：");
            fflush(stdin);
            str_input(&str);
            length = str_len(&str);
            printf("字符串：%s的长度为%d\n", str.string, length);
            system("pause");
            system("cls");
            break;
        case 2:
            printf("请输入源字符串：");
            fflush(stdin);
            str_input(&str1);
            printf("请输入替换字符串：");
            fflush(stdin);
            str_input(&str2);
            printf("请输入替换位置：");
            scanf("%d", &position);
            str_rep(&str1, &str2, position);
            printf("替换后的结果为：");
            str_print(&str1);
            system("pause");
            system("cls");
            break;
        case 3:
            printf("请输入第一串字符：");
            fflush(stdin);
            str_input(&str1);
            printf("请输入第二串字符串：");
            fflush(stdin);
            str_input(&str2);
            str_con(&str1, &str2);
            printf("两串字符连接后的结果为：");
            str_print(&str1);
            system("pause");
            system("cls");
            break;
        case 4:
            printf("请输入一串字符：");
            fflush(stdin);
            str_input(&str1);
            printf("请输入取子串的位置和长度：");
            scanf("%d%d", &position, &length);
            str_sub(&str, &str1, position, length);
            printf("子串为：");
            puts(str.string);
            system("pause");
            system("cls");
            break;
        case 5:
            exit(0);
            return 0;
        default:
            printf("输入错误，请重新输入！\n");
            system("pause");
            system("cls");
        }
    }
}
