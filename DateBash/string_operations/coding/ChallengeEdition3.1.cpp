/*
 * @Description:
 * @Version: 2.0
 * @Autor: kerwinz
 * @Date: 2023-11-14 12:20:07
 * @LastEditors: kerwinz
 * @LastEditTime: 2023-11-16 22:02:08
 */
#include <stdio.h>
#include <ctype.h>
#include <locale.h>

#define UTF8_LEAD_1 0xE0 // UTF8中文字符第一个字节
#define UTF8_LEAD_2 0x80 // UTF8中文字符后两个字节的前两位
#define ANSI 2
#define UTF8 3

bool checkChinese(int ch)
{
    return ch & 0x80;
}

int detectEncoding(FILE *fp)
{
    unsigned char bom[3];
    fread(bom, sizeof(unsigned char), 3, fp);
    // 检查 UTF-8 字节序列的 BOM
    if (bom[0] == 0xEF && bom[1] == 0xBB && bom[2] == 0xBF)
    {
        return UTF8; // 存在 BOM，则为 UTF-8 编码
    }
    // 重置文件指针
    fseek(fp, 0, SEEK_SET);
    // 判断 ASCII 编码
    int ch;
    while ((ch = fgetc(fp)) != EOF)
    {
        if (ch > 127)
        {
            return UTF8; // 存在非 ASCII 字符，判断为 UTF-8 编码
        }
    }
    return ANSI; // 全部为 ASCII 字符，判断为 ASCII 编码
}

int main()
{

    FILE *fp = fopen("text.txt", "r");

    int chars = 0;         // 统计字符总数
    int spaces = 0;        // 统计空格数量
    int letters = 0;       // 统计字母数量
    int digits = 0;        // 统计数字数量
    int punctuations = 0;  // 统计标点符号数量
    int chinese_chars = 0; // 统计中文字符数量
    int words = 0;         // 统计单词数量
    int lines = 1;         // 统计行数（包括空行）
    int paragraphs = 1;    // 统计段落数（以换行符分隔）
    int chinesebyte = 0;   // 统计中文字符所占的字节数

    int ch;
    int last_ch = 0;
    int next_ch = 0;

    while ((ch = fgetc(fp)) != EOF)
    {
        chars++;
        if (ch == ' ')
        {
            spaces++;
        }
        else if (isalpha(ch))
        {
            letters++;
        }
        else if (isdigit(ch))
        {
            digits++;
        }
        else if (ispunct(ch))
        {
            punctuations++;
        }
        else if (checkChinese(ch))
        {
            chinese_chars++;
            printf("%c", ch);
        }

        if (ch == '\n')
        {
            lines++;
            paragraphs++;
        }
        else if (isspace(ch) || (isalpha(last_ch) && ispunct(ch)))
        {
            words++;
        }
        last_ch = ch;
    }

    chinesebyte = detectEncoding(fp);
    printf("\nchinsebyte: %d\n", chinesebyte);
    printf("Total chars: %d\n", chars);
    printf("Total spaces: %d\n", spaces);
    printf("Total letters: %d\n", letters);
    printf("Total digits: %d\n", digits);
    printf("Total punctuations: %d\n", punctuations);
    printf("Total Chinese chars: %d\n", chinese_chars / chinesebyte);
    printf("Total words: %d\n", words);
    printf("Total lines: %d\n", lines);
    printf("Total paragraphs: %d\n", paragraphs);

    fclose(fp);

    return 0;
}