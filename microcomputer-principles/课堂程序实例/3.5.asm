; ------------------------------------------------------------------
; @Author: kerwin-win zhangjszs@foxmail.com
; @Date: 2024-04-02
; @LastEditors: kerwin-win
; @Description: 课堂示例 3.5（从键盘读取两个字符并相加显示）
; @Notes: 保持原实现，仅添加注释
; ------------------------------------------------------------------
DATAS SEGMENT
      var1  db ?
      var2  db ?
DATAS ENDS

STACKS SEGMENT
      ;�˴������ջ�δ���
STACKS ENDS

CODES SEGMENT
            ASSUME CS:CODES,DS:DATAS,SS:STACKS
      START:
            MOV    AX,DATAS
            MOV    DS,AX
      ;����
            MOV    AH,01
            INT    21H
            MOV    var1,AL
    
            MOV    AH,01
            INT    21H
            ADD    AL,var1
    
      ;��ʾ
            MOV    DL,AL
            MOV    AH,02H
            INT    21H
    
      ;�˴��������δ���
            MOV    AH,4CH
            INT    21H
CODES ENDS
    END START
