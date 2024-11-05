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
      ;
            MOV    AL,00101011B
            MOV    DL,10100010B
            ADD    AL,DL
      ;
            MOV    AL,036H
            AND    AL,00FH
      ;
            MOV    AL,006H
            OR     AL,030H
      ;
            MOV    AL ,10010011B
            SHL    AL,1
    
            MOV    AL,00010011B
            MOV    CL,2
            SHL    AL,CL
    
            MOV    AL, 00010011B
            MOV    BL,4
            MUL    BL
      ;
            MOV    AL ,10010011B
            SHR    AL,1
    
            MOV    AL,10010011B
            MOV    CL,2
            SHR    AL,CL
    
            MOV    AX, 10010011B
            MOV    BL,4
            DIV    BL
    
      ;
            MOV    AL ,10010011B
            SAL    AL,1
    
            MOV    AL,10010011B
            MOV    CL,2
            SAL    AL,CL
    
      ;
            MOV    AL ,10010011B
            SAR    AL,1
    
            MOV    AL,10010011B
            MOV    CL,2
            SAR    AL,CL
    
      ;
            MOV    AL ,10010011B
            ROL    AL,1
    
            MOV    AL,10010011B
            ROR    AL,1
    
            MOV    AL,10010011B
            RCL    AL,1
    
            MOV    AL,10010011B
            RCR    AL,1
      ;�˴��������δ���
            MOV    AH,4CH
            INT    21H
CODES ENDS
    END START

