DATAS SEGMENT
      ;�˴��������ݶδ���
      var1  db 031H
      var2  db 032H
      var3  db 033H
      var4  db 034H
      var5  db 035H
      var6  db 036H
      var7  db 037H
      var8  db 038H
      var9  db 039H
      varA  dw 041H
      varB  dw 042H
      ;������
      table db 061H,062H,064H,065H,066H,067H,068H,069H,06AH,06BH,06CH,06DH,06EH,06FH
DATAS ENDS

STACKS SEGMENT
      ;�˴������ջ�δ���
             db 32 dup(?)
STACKS ENDS

CODES SEGMENT
            ASSUME CS:CODES,DS:DATAS,SS:STACKS
      START:
            MOV    AX,DATAS
            MOV    DS,AX
    
            MOV    AX,STACKS
            MOV    SS,AX
      ;�˴��������δ���
            MOV    AL,var1
            MOV    AH,var2
            MOV    var1,AH
            MOV    var2,AL
      ;
            MOV    AL,var3
            XCHG   AL,var4
            MOV    var3,AL
      ;
            MOV    AX,varA
            PUSH   AX
            MOV    AX,varB
            PUSH   AX
            POP    AX
            MOV    varA,AX
            POP    AX
            MOV    varB,AX
      ;
            LEA    BX,table
            MOV    AL,03
            XLAT
    
      ;
    
            MOV    AH,4CH
            INT    21H
CODES ENDS
    END START

