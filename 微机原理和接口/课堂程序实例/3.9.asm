DATAS SEGMENT
    ;�˴��������ݶδ���
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
           MOV    AL,01H
           ADD    AH,AL
           JMP    SECOND
    FIRST: 
           MOV    AL,02H
           ADD    AH,AL
    SECOND:
           MOV    AL,03H
           ADD    AH,AL
     
           JMP    FIRST
           MOV    AH,4CH
           INT    21H
CODES ENDS
    END START
