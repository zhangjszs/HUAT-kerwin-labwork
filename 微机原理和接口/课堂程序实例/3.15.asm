; ------------------------------------------------------------------
; @Author: kerwin-win zhangjszs@foxmail.com
; @Date: 2024-04-02
; @LastEditors: kerwin-win
; @Description: 课堂示例 3.15（带条件跳转的累加示例）
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
              MOV    AH,n
    STARTLOOP:
              CMP    DL,AH
              JNBE   ENDLOOP

              ADD    AL,DL
              INC    DL
	 
              JMP    STARTLOOP
    ENDLOOP:  
              MOV    SUM,AL
              MOV    SUM,AL
              MOV    AH,4CH
              INT    21H
CODES ENDS
    END START

