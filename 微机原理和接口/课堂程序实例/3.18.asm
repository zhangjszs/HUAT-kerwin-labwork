DATAS SEGMENT
    ;�˴��������ݶδ���
    v1    DB 123
DATAS ENDS

STACKS SEGMENT
    ;�˴������ջ�δ���
STACKS ENDS

CODES SEGMENT
              ASSUME CS:CODES,DS:DATAS,SS:STACKS
    START:    
              MOV    AX,DATAS
              MOV    DS,AX
              MOV    AL,v1
    LOOPSTART:
              CBW
              MOV    BL,10
              DIV    BL
              MOV    DL,AH
              ADD    DL,30H
              MOV    AH,02H
              MOV    v1,AL
              INT    21H
              MOV    AL,v1
              CMP    AL,0H
              JNE    LOOPSTART
    
    ;�˴��������δ���
              MOV    AH,4CH
              INT    21H
CODES ENDS
    END START
