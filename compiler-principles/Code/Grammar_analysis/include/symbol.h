#ifndef _SYMBOL_H
#define _SYMBOL_H

#include <cstdio>
#include <cstdlib>

const int WORDLEN = 14;   // 保留字个数
const int MAXIDLEN = 50;  // 标识符最长长度
const int SYMBOLNUM = 35; // 种别码个数

enum SYMBOL {
    NOL, IDENT, NUMBER, PLUS, MINUS,
    TIMES, SLASH, ODDSYM, EQL, NEQ,
    LSS, LEQ, GTR, GEQ, LPAREN,
    RPAREN, COMMA, SEMICOLON, PERIOD, BECOMES,
    BEGINSYM, ENDSYM, IFSYM, THENSYM, WHILESYM,
    WRITESYM, READSYM, DOSYM, CALLSYM, CONSTSYM,
    VARSYM, PROCSYM, LBRACKET, RS, MAINSYM
}; // 定义种别码

void Getchar();       // 取下一个字符
void Getbc();         // 如当前字符为白字符，则读字符至不为白字符时停止
void Concat();        // 将当前字符加入token字
void Retract();       // 回退一字符
int Reserve();        // 判断token字中单词是否是保留字
int Getsym();         // 从当前文件中识别出一单词，并给出其类型和值
void Errorsym();      // 打印错误信息

#endif // _SYMBOL_H



