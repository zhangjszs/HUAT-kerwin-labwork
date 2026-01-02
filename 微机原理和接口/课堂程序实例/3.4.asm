; ------------------------------------------------------------------
; @Author: kerwin-win zhangjszs@foxmail.com
; @Date: 2024-04-02
; @LastEditors: kerwin-win
; @Description: 课堂示例 3.4（数据表示、常量与字符串演示）
; @Notes: 保持原实现，仅添加注释
; ------------------------------------------------------------------
DATAS SEGMENT
    x     db  10
    y     db  10H
    z     db  10B
    xx    dw  1010
    yy    dw  1010H
    zz    dw  1010B

    str1  DB  'abcdefg','$'
COUNT= $-str1
    LEN   EQU 5
    PI    EQU 3.4
    t     dw  LEN

DATAS ENDS

STACKS SEGMENT
    ;�˴������ջ�δ���
STACKS ENDS

CODES SEGMENT
          ASSUME CS:CODES,DS:DATAS,SS:STACKS
    START:
          MOV    AX,DATAS
          MOV    DS,AX
          MOV    AL,x
          MOV    AH,y
          MOV    AX,XX
          MOV    BX, LEN
          MOV    CL,COUNT
          MOV    CH,LEN
    ;LEN EQU 6
    ;COUNT=10
          MOV    CH,BYTE PTR xx
    ;MOV DX,PI
    ;MOV AH,XX
    ;MOV AX,x
    ;MOV AX,WORD PTR x
    ;�˴��������δ���
          MOV    AH,4CH
          INT    21H
CODES ENDS
    END START




