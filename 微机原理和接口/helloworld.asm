; ------------------------------------------------------------------
; @Author: kerwin-win zhangjszs@foxmail.com
; @Date: 2024-04-02
; @LastEditors: kerwin-win
; @Description: Hello World 示例（显示字符串）
; @Notes: 仅添加注释，保持原实现不变
; ------------------------------------------------------------------
DATA SEGMENT
    PRINT DB "Hello World!", 0AH, 0DH, '$'
DATA ENDS
STACK SEGMENT   STACK
          DW 20  DUP(0)
STACK ENDS
CODE SEGMENT
          ASSUME CS:CODE, DS:DATA, SS:STACK
    START:
          MOV    AX, DATA
          MOV    DS, AX
          MOV    DX, OFFSET  PRINT
          MOV    AH, 09
          INT    21H
          MOV    AH, 4CH
          INT    21H
CODE ENDS
END    START
