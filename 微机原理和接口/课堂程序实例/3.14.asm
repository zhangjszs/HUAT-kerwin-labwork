DATAS SEGMENT
    ;�˴��������ݶδ���
    sum   db 0H
    n     db 5H
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
              MOV    CL,n
              MOV    AL,0
              MOV    DL,1
    STARTLOOP:
              ADD    AL,DL
              INC    DL
              SUB    CL,1
              JNE    STARTLOOP
    ENDLOOP:  
              MOV    SUM,AL
              MOV    AH,4CH
              INT    21H
CODES ENDS
    END START

