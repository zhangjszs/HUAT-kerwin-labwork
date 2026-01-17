; ------------------------------------------------------------------
; @Author: kerwin-win zhangjszs@foxmail.com
; @Date: 2024-04-02
; @LastEditors: kerwin-win
; @Description: 课堂示例 3.18_3（将 16 进制数转换为 ASCII 字符串并显示）
; @Notes: 使用循环与位操作生成字符串
; ------------------------------------------------------------------
DATAS SEGMENT
    v1      DW 4678H
    datastr DB 10 dup(?)
DATAS ENDS

STACKS SEGMENT
    ;�˴������ջ�δ���
STACKS ENDS

CODES SEGMENT
              ASSUME CS:CODES,DS:DATAS,SS:STACKS
    START:    
              MOV    AX,DATAS
              MOV    DS,AX
              MOV    AX,v1
              MOV    CL,4
              LEA    SI,datastr
    LOOPSTART:
              MOV    BL,CL
              MOV    CL,4
              ROL    AX,CL
              MOV    DL,AL
              AND    DL,0FH
              OR     DL,30H
              MOV    [SI],DL
              MOV    CL,BL
              INC    SI
              LOOP   LOOPSTART
              MOV    AL,'$'
              MOV    [SI],AL
              LEA    DX,datastr
              MOV    AH,09H
              INT    21H
    
    ;�˴��������δ���
              MOV    AH,4CH
              INT    21H
CODES ENDS
    END START





