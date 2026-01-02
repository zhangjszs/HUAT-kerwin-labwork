; ------------------------------------------------------------------
; @Author: kerwin-win zhangjszs@foxmail.com
; @Date: 2024-04-02
; @LastEditors: kerwin-win
; @Description: 课堂示例 3.10（判断输入是否为字符 'a' 并输出相应提示）
; @Notes: 演示条件跳转 JE 与字符串输出
; ------------------------------------------------------------------
DATAS SEGMENT
      STRING1 DB 'INPUT char a!',13,10,'$'
      STRING2 DB 'INPUT other char!',13,10,'$'
DATAS ENDS
; 判断输入字符是否为 'a'
CODES SEGMENT
            ASSUME CS:CODES,DS:DATAS
      START:
            MOV    AX,DATAS
            MOV    DS,AX
     
            MOV    AH,01H
            INT    21H
     
            SUB    AL,'a'
            JE     LOAD1
            LEA    DX,STRING2
            JMP    SHOW
      LOAD1:
            LEA    DX,STRING1
      SHOW: 
            MOV    AH,9
            INT    21H
   
            MOV    AH,4CH
            INT    21H
CODES ENDS
    END   START






