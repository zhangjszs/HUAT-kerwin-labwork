; ------------------------------------------------------------------
; @Author: kerwin-win zhangjszs@foxmail.com
; @Date: 2024-04-02
; @LastEditors: kerwin-win
; @Description: 课堂示例 3.18Fun（定义换行宏并演示数字输出函数）
; @Notes: crlf 宏用于输出回车换行，ShowFun 为打印十进制数字的子程序
; ------------------------------------------------------------------
crlf macro             ; 换行宏
         mov ah,2
         mov dl,0dh    ; 回车
         int 21h

         mov ah,2
         mov dl,0ah    ; 换行
         int 21h
endm

DATAS SEGMENT
    v1    DB 123
    v2    DB 105
DATAS ENDS

STACKS SEGMENT
    ;�˴������ջ�δ���
           db 100 dup(?)
STACKS ENDS

CODES SEGMENT
              ASSUME CS:CODES,DS:DATAS,SS:STACKS
    START:    
              MOV    AX,DATAS
              MOV    DS,AX
              MOV    AL,v1
              CALL   ShowFun
    
   
    
              MOV    AL,v2
              CALL   ShowFun
    ;�˴��������δ���
              MOV    AH,4CH
              INT    21H

ShowFun Proc
    LOOPSTART:
              CBW
              MOV    BL,10
              DIV    BL
              MOV    DL,AH
              ADD    DL,30H
              push   AX
              MOV    AH,02H
    
              INT    21H
              POP    AX
              CMP    AL,0H
              JNE    LOOPSTART
              ret
ShowFun endp

CODES ENDS
    END START




