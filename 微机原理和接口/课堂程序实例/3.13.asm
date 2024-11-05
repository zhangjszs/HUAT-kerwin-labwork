DATAS SEGMENT
    ;�˴��������ݶδ���
    var1  db 04H
    var2  db 02H
    var3  db ?
    MSG   DB 'ERROR CHAR!',13,10,'$'
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
            MOV    AH,01H
            INT    21H
            MOV    AH,AL
            SUB    AL,'+'
            JE     ADDDATA
            MOV    AL,AH
            SUB    AL,'-'
            JE     SUBDATA
            MOV    AL,AH
            SUB    AL,'*'
            JE     MULDATA
            MOV    AL,AH
            SUB    AL,'/'
            JE     DIVDATA
            JMP    OTHER
    ADDDATA:
            MOV    DL,var1
            MOV    DH,var2
            ADD    DL,DH
            JMP    SHOW
    SUBDATA:
            MOV    DL,var1
            MOV    DH,var2
            SUB    DL,DH
            JMP    SHOW
    MULDATA:
            MOV    DL,var1
            MOV    AL,var2
            MUL    DL
            MOV    DL,AL
            JMP    SHOW
    DIVDATA:
            MOV    AH,0H
            MOV    AL,var1
            MOV    DL,var2
            DIV    DL
            MOV    DL,AL
            JMP    SHOW
    SHOW:   
            MOV    var3,DL
            OR     DL,030H
            MOV    AH,2
            INT    21H
            JMP    ENDPRO
    OTHER:  
            LEA    DX,MSG
            MOV    AH,9
            INT    21H
            JMP    ENDPRO
    ENDPRO: 
     
            MOV    AH,4CH
            INT    21H
CODES ENDS
    END START



