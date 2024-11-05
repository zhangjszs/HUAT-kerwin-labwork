  
IOPORT  EQU       0D100H-0280H
    OUTPORT1  EQU       IOPORT+2A0H
    OUTPORT2  EQU       IOPORT+2A8H
CODE SEGMENT
          ASSUME CS:CODE
    START:
          MOV    DX,OUTPORT1
          OUT    DX,AL
          CALL   DELAY          ;调延时子程序
          MOV    DX,OUTPORT2
          OUT    DX,AL
          CALL   DELAY          ;调延时子程序
          MOV    AH,1
          INT    16H
          JE     START
          MOV    AH,4CH
          INT    21H
DELAY PROC      NEAR            ;延时子程序
          MOV    BX,2000
    LLL:  MOV    CX,0
    LL:   LOOP   LL
          DEC    BX
          JNE    LLL
          RET
DELAY ENDP
CODE ENDS
              END       START