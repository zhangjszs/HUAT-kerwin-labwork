; ------------------------------------------------------------------
; @Author: kerwin-win zhangjszs@foxmail.com
; @Date: 2024-04-02
; @LastEditors: kerwin-win
; @Description: 课堂示例 3.11（判断是否为小写字母并输出不同提示）
; @Notes: 与 3.12 相似，展示条件分支用法
; ------------------------------------------------------------------
DATAS SEGMENT
      STRING1 DB 'INPUT LOWER CHAR!',13,10,'$'
      STRING2 DB 'INPUT OTHER char!',13,10,'$'
DATAS ENDS
; 判断输入字符是否小写字母
CODES SEGMENT
            ASSUME CS:CODES,DS:DATAS
      START:
            MOV    AX,DATAS
            MOV    DS,AX
     
            MOV    AH,01H
            INT    21H
     
            SUB    AL,'a'
            JL     LOAD1
            SUB    AL,'z'
            JNL    LOAD1
            LEA    DX,STRING1
            JMP    SHOW
      LOAD2:
            LEA    DX,STRING2
      SHOW: 
            MOV    AH,9
            INT    21H
   
            MOV    AH,4CH
            INT    21H
CODES ENDS
    END   START
