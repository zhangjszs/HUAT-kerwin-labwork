; ------------------------------------------------------------------
; @Author: kerwin-win zhangjszs@foxmail.com
; @Date: 2024-04-02
; @LastEditors: kerwin-win
; @Description: 课堂示例 3.16（累加求和示例）
; @Notes: 使用 LOOP 循环累加 1..n 的和
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
              MOV    CH,0
              MOV    AL,0
              MOV    DL,1
    STARTLOOP:
     
              ADD    AL,DL
              INC    DL
	 
              LOOP   STARTLOOP
	
              MOV    SUM,AL
              MOV    AH,4CH
              INT    21H
CODES ENDS
    END START

