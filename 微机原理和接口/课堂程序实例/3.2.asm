; ------------------------------------------------------------------
; @Author: kerwin-win zhangjszs@foxmail.com
; @Date: 2024-04-02
; @LastEditors: kerwin-win
; @Description: 课堂示例 3.2（简单加法并输出）
; @Notes: 保持原实现，仅添加注释
; ------------------------------------------------------------------
DATAS SEGMENT
      one   DB 01H
      two   DB 02H
DATAS ENDS

STACKS SEGMENT
      ;�˴������ջ�δ���
STACKS ENDS

CODES SEGMENT
            ASSUME CS:CODES,DS:DATAS,SS:STACKS
      START:
            MOV    AX,DATAS
            MOV    DS,AX
    
            MOV    AL,one
            ADD    AL,two
    
            MOV    DL,AL
      ;ADD DL,30H
            Mov    AH,2H
            INT    21H
      ;�˴��������δ���
            MOV    AH,4CH
            INT    21H
CODES ENDS
    END START
