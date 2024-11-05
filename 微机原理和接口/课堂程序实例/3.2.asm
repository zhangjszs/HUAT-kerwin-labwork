DATAS SEGMENT
      one   DB 01H
      two   DB 02H
DATAS ENDS

STACKS SEGMENT
      ;�˴������ջ�δ���
STACKS ENDS

CODES SEGMENT
            ASSUME CS:CODES,DS:DATAS,SS:STACKS
      START:
            MOV    AX,DATAS
            MOV    DS,AX
    
            MOV    AL,one
            ADD    AL,two
    
            MOV    DL,AL
      ;ADD DL,30H
            Mov    AH,2H
            INT    21H
      ;�˴��������δ���
            MOV    AH,4CH
            INT    21H
CODES ENDS
    END START
