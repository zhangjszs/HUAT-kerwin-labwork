; ------------------------------------------------------------------
; @Author: kerwin-win zhangjszs@foxmail.com
; @Date: 2024-04-02
; @LastEditors: kerwin-win
; @Description: 课堂示例 3.3（数据访问与寻址模式示例）
; @Notes: 保持原实现，仅添加注释
; ------------------------------------------------------------------
DATAS SEGMENT
      one   DB 41H
      two   DB 42H
            DB 43H,44H,45H,46H,47H,48H,49H,4AH,4BH,4CH,4DH,4EH,4FH
            DB 15 DUP (? )
            DB 25 DUP(07H)
      table db 01H,02H,03H,04H,05H,06H,07H,08H,09H
      index db 0H
DATAS ENDS

STACKS SEGMENT
      ;�˴������ջ�δ���
STACKS ENDS

CODES SEGMENT
            ASSUME CS:CODES,DS:DATAS,SS:STACKS
      START:
            MOV    AX,DATAS
            MOV    DS,AX
    
            MOV    AX,07AEH
    
            MOV    AL,one
            MOV    AH,two
   
            MOV    AL,[0005H]
    
            MOV    DL,DS:[0005H]
    
    
            MOV    BX,offset one
            ADD    BX,3H
            MOV    AL,[BX]
    
            MOV    AH,[BX+2]
    
            MOV    AH,3[BX]                         ;ע�� MOV AH,[BX]4
    
            MOV    SI,02H
            MOV    AH,[BX+SI+06H]
            MOV    AH,[BX][SI][07H]
            MOV    AH,08H[BX][SI]
      ;�˴��������δ���
    
            mov    BX,offset table
            mov    AH,1[BX]
            add    AH,2[BX]
            add    AH,3[BX]
    
            MOV    AH,4CH
            INT    21H
CODES ENDS
    END START




