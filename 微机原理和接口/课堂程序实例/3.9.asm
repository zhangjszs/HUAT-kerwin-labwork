; ------------------------------------------------------------------
; @Author: kerwin-win zhangjszs@foxmail.com
; @Date: 2024-04-02
; @LastEditors: kerwin-win
; @Description: 课堂示例 3.9（寄存器运算示例）
; @Notes: 保持原实现，仅添加注释以便学习
; ------------------------------------------------------------------
DATAS SEGMENT
       ; 数据段
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
              MOV    AL,01H
              ADD    AH,AL
              JMP    SECOND
       FIRST: 
              MOV    AL,02H
              ADD    AH,AL
       SECOND:
              MOV    AL,03H
              ADD    AH,AL
     
              JMP    FIRST
              MOV    AH,4CH
              INT    21H
CODES ENDS
    END START
