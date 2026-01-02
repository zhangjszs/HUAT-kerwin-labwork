; ------------------------------------------------------------------
; @Author: kerwin-win zhangjszs@foxmail.com
; @Date: 2024-04-02
; @LastEditors: kerwin-win
; @Description: 课堂示例 3.14（使用 LOOP 和 SUB 的累加示例）
; @Notes: 保持原实现，仅添加注释
; ------------------------------------------------------------------
DATAS SEGMENT
    sum   db 0H
    n     db 5H
DATAS ENDS

STACKS SEGMENT
    ;�˴������ջ�δ���
STACKS ENDS

CODES SEGMENT
              ASSUME CS:CODES,DS:DATAS,SS:STACKS
    START:    
              MOV    AX,DATAS
              MOV    DS,AX
    ;�˴��������δ���
              MOV    CL,n
              MOV    AL,0
              MOV    DL,1
    STARTLOOP:
              ADD    AL,DL
              INC    DL
              SUB    CL,1
              JNE    STARTLOOP
    ENDLOOP:  
              MOV    SUM,AL
              MOV    AH,4CH
              INT    21H
CODES ENDS
    END START

