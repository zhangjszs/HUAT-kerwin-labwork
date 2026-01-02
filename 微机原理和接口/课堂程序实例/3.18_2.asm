; ------------------------------------------------------------------
; @Author: kerwin-win zhangjszs@foxmail.com
; @Date: 2024-04-02
; @LastEditors: kerwin-win
; @Description: 课堂示例 3.18_2（循环左移并打印二进制位）
; @Notes: 使用 SHL 和标志位 JC 判定最高位
; ------------------------------------------------------------------
DATAS SEGMENT
    v1    DB 10101101B
DATAS ENDS

STACKS SEGMENT
    ;�˴������ջ�δ���
STACKS ENDS

CODES SEGMENT
              ASSUME CS:CODES,DS:DATAS,SS:STACKS
    START:    
              MOV    AX,DATAS
              MOV    DS,AX
              MOV    AL,v1
    LOOPSTART:
	
              SHL    AL,1
              MOV    v1,AL
              JC     SHOW1
              MOV    DL,30H
              JMP    SHOW
    SHOW1:    
              MOV    DL,31H
    SHOW:     
              MOV    AH,02H
              INT    21H
              MOV    AL,v1
              CMP    AL,0H
              JNE    LOOPSTART
    
    ;�˴��������δ���
              MOV    AH,4CH
              INT    21H
CODES ENDS
    END START
