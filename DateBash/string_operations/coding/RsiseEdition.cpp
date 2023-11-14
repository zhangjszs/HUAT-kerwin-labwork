#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
    char ch;
    struct node *next;
} STRNODE;

int str_len(STRNODE *s)
{
    return s->ch;
}

void str_input(STRNODE *s)
{
    // int i = 0;
    char ch;
    STRNODE *p, *q;
    p = s;
    q = s->next;
    s->ch = 0;
    while ((ch = getchar()) != '\n')
    {
        q = (STRNODE *)malloc(sizeof(STRNODE));
        q->ch = ch;
        q->next = NULL;
        p->next = q;
        p = q;
        s->ch++;
    }
}

STRNODE *str_rep(STRNODE *s1, STRNODE *s2, int pos)
{
    int i = 1;
    STRNODE *p, *q;
    p = s1->next;
    q = s2->next;
    while (p != NULL && i != pos)
    {
        i++;
        p = p->next;
    }
    for (i = 1; i <= s2->ch; i++)
    {
        p->ch = q->ch;
        p = p->next;
        q = q->next;
    }
    return s1;
}

STRNODE *str_con(STRNODE *s1, STRNODE *s2)
{
    STRNODE *p = s1->next;
    while (p->next != NULL)
    {
        p = p->next;
    }
    p->next = s2->next;
    s1->ch += s2->ch;
    free(s2);
    return s1;
}

STRNODE *str_sub(STRNODE *sub, STRNODE *s, int pos, int len)
{
    STRNODE *p, *q;
    p = s->next;
    int i = 1;
    while (p != NULL && i != pos)
    {
        p = p->next;
        i++;
    }
    if (p == NULL)
    {
        return sub;
    }
    q = sub;
    for (i = 0; i < len; i++)
    {
        STRNODE *temp = (STRNODE *)malloc(sizeof(STRNODE));
        temp->ch = p->ch;
        temp->next = NULL;
        q->next = temp;
        q = temp;
        p = p->next;
    }
    return sub;
}

void str_print(STRNODE *s)
{
    STRNODE *p = s->next;
    while (p != NULL)
    {
        putchar(p->ch);
        p = p->next;
    }
    printf("\n");
}

void showmenu()
{
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
    STRNODE str, str1, str2;
    str.ch = 0;
    str1.ch = 0, str2.ch = 0;
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
            printf("字符串：");
            str_print(&str);
            printf("长度为:%d\n", length);
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
            STRNODE sub;
            sub.ch = 0;
            STRNODE *result = str_sub(&sub, &str1, position, length);
            printf("子串为：");
            str_print(result);
            system("pause");
            system("cls");
            break;
        case 5:
            return 0;
        }
    }
    return 0;
}