#include "basedata.h"
#include "symbol.h"
#include <cctype>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <iomanip>
#include <string.h>

const char* WORD[WORDLEN] = {
    "BEGIN", "CALL", "CONST", "DO", "END",
    "IF", "ODD", "PROCEDURE", "READ", "THEN",
    "VAR", "WHILE", "WRITE", "MAIN"
}; // 保留字字符串表,用于将保留字种别码转为字符串输出

SYMBOL WSYM[WORDLEN] = {
    BEGINSYM, CALLSYM, CONSTSYM, DOSYM, ENDSYM,
    IFSYM, ODDSYM, PROCSYM, READSYM, THENSYM,
    VARSYM, WHILESYM, WRITESYM, MAINSYM
}; // 保留字种别码表

const char* SNAME[SYMBOLNUM] = {
    "NOL", "IDENT", "NUMBER", "PLUS", "MINUS",
    "TIMES", "SLASH", "ODDSYM", "EQL", "NEQ",
    "LSS", "LEQ", "GTR", "GEQ", "LPAREN",
    "RPAREN", "COMMA", "SEMICOLON", "PERIOD", "BECOMES",
    "BEGINSYM", "ENDSYM", "IFSYM", "THENSYM", "WHILESYM",
    "WRITESYM", "READSYM", "DOSYM", "CALLSYM", "CONSTSYM",
    "VARSYM", "PROCSYM", "LBRACKET", "RS", "MAINSYM"
}; // 单词字符串表,用于将保留字种别码转为字符串输出

SYMBOL sym; // 最近已识的单词种别码
char token[MAXIDLEN + 1]; // 最近已识别的单词
int num; // 最近已识别的数字值
char ch; // 最近已识别的字符
int col = 1, row = 1; // 当前行和列值
FILE* fd; // 指向待编译文件
extern std::ofstream fout; // 指向存放结果文件

void Getchar() {
    ch = fgetc(fd);
    if (ch != EOF && ch != '\n') {
        col++;
    }
}

void Getbc() {
    while (ch == SPACE || ch == TABLE || ch == '\n') {
        if (ch == '\n') {
            row++;
            col = 1;
        }
        Getchar();
    } // 为空字符则一直读至不为空字符
}

void Retract() {
    fseek(fd, -1l, SEEK_CUR);
    col--;
}

void Concat() {
    char temp[2];
    temp[0] = ch;
    temp[1] = '\0';
    strcat(token, temp);
}

int Reserve() {
    int i, j;
    char temp[60];
    j = strlen(token);
    for (i = 0; i < j; i++) {
        temp[i] = toupper(token[i]); // 将当前token字以大写形式存入temp中
    }
    temp[i] = '\0';
    for (i = 0; i < WORDLEN; i++) {
        if (!strcmp(WORD[i], temp)) {
            break;
        }
    } // 判断当前token是否是保留字
    if (i >= WORDLEN) i = -1;
    return i;
}

void Errorsym() {
    fout << "There is error @row: " << std::setw(5) << row << ", @col: " << std::setw(5) << col << std::endl;
}

int Getsym() {
    int k;
    int flag = TRUE;
    Getchar();
    Getbc(); // 滤掉白字符
    strcpy(token, "");
    if (isalpha(ch)) {
        // 以字母开头则是标识符
        num = 0;
        Concat();
        Getchar();
        while (isalnum(ch)) {
            Concat();
            Getchar();
        }
        Retract(); // 由于超前搜索了，所以回退一个字符
        k = Reserve(); // 判断此标识符是否是保留字
        if (k != -1) {
            sym = WSYM[k]; // 将保留字种别码存入sym中
        } else {
            sym = IDENT; // 将一般标识符种别码存入sym中
        } // end else k!=-1;
    } // end of if isalpha
    else if (isdigit(ch)) {
        // 以数字开头则为无符号整数
        Concat();
        Getchar();
        while (isdigit(ch)) {
            Concat();
            Getchar();
        }
        if (isalpha(ch)) {
            flag = FALSE;
            while (isalnum(ch)) {
                Concat();
                Getchar();
            }
        } // end of flag=FALSE
        Retract(); // 回退
        if (flag) { // 若是无符号整数，则将整数值存于num中
            sym = NUMBER;
            num = atoi(token);
        }
    } // end of if isdigit
    else {
        num = 0;
        switch (ch) {
        case '+': Concat(); sym = PLUS; break;
        case '-': Concat(); sym = MINUS; break;
        case '*': Concat(); sym = TIMES; break;
        case '/': Concat(); sym = SLASH; break;
        case '(': Concat(); sym = LPAREN; break;
        case ')': Concat(); sym = RPAREN; break;
        case '=': Concat(); sym = EQL; break;
        case '#': Concat(); sym = NEQ; break;
        case '{': Concat(); sym = LBRACKET; break;
        case '\\': Concat(); sym = RS; break;
        /*
           ODDSYM,EQL,NEQ,LSS,LEQ,GTR,GEQ,LPAREN,
           RPAREN,COMMA,SEMICOLON,PERIOD,BECOMES,
        */
        case ',': Concat(); sym = COMMA; break;
        case '.': Concat(); sym = PERIOD; break;
        case ';': Concat(); sym = SEMICOLON; break;
        case '>':
            Concat();
            Getchar();
            if (ch != '=') { // 若后不跟'=',则回退
                sym = GTR;
                Retract();
            } else {
                Concat();
                sym = GEQ;
            }
            break;
        case '<':
            Concat();
            Getchar();
            if (ch != '=') {
                sym = LSS;
                Retract();
            } else {
                Concat();
                sym = LEQ;
            }
            break;
        case ':':
            Concat();
            Getchar();
            if (ch != '=') {
                flag = FALSE;
                Retract();
            } else {
                Concat();
                sym = BECOMES;
            }
            break;
        default:
            Concat();
            flag = FALSE;
            break;
        } // end of switch else char
    } // end of else char
    num = sym;
    return flag;
}



