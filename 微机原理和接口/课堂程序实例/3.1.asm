DATAS SEGMENT
   
DATAS ENDS

STACKS SEGMENT
      ;�˴������ջ�δ���
STACKS ENDS

CODES SEGMENT
            ASSUME CS:CODES,DS:DATAS,SS:STACKS
      START:
      ;MOV AX,DATAS
      ;MOV DS,AX

      ;MOV DL,1H
            MOV    DL,31H
            Mov    AH,2H
            MOV    AX,DATAS
            MOV    DS,AX
            INT    21H
      ;�˴��������δ���
            MOV    AH,4CH
            INT    21H
CODES ENDS
    END START

