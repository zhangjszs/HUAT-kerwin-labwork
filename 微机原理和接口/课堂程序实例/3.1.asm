; ------------------------------------------------------------------
; @Author: kerwin-win zhangjszs@foxmail.com
; @Date: 2024-04-02
; @LastEditors: kerwin-win
; @Description: 课堂示例 3.1（简单的字符串/字符输出示例）
; @Notes: 使用 INT 21H 的 AH=02 功能输出字符
; ------------------------------------------------------------------
DATAS SEGMENT
   
DATAS ENDS

STACKS SEGMENT
      ;�˴������ջ�δ���
STACKS ENDS

CODES SEGMENT
            ASSUME CS:CODES,DS:DATAS,SS:STACKS
      START:
      ;MOV AX,DATAS
      ;MOV DS,AX

      ;MOV DL,1H
            MOV    DL,31H
            Mov    AH,2H
            MOV    AX,DATAS
            MOV    DS,AX
            INT    21H
      ;�˴��������δ���
            MOV    AH,4CH
            INT    21H
CODES ENDS
    END START

