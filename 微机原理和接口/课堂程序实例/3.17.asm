DATAS SEGMENT
    ;�˴��������ݶδ���
    array db 1H,2H,3H,4H,5H,6H,7H,8H,9H,10H
len=$-array
    p1    db 0H
    p2    db 0H
    
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
              MOV    CX,len
              MOV    DL,0
              MOV    DH,0
              LEA    SI,array
    STARTLOOP:
              MOV    AL,[SI]
              TEST   AL,001H
              JNZ    P1ADD
    ;SHR AL,1
              INC    DH
              JNZ    ENDADD
    P1ADD:    
              INC    DL
    ENDADD:   
              INC    SI
	 
              LOOP   STARTLOOP
	
              MOV    p1,DL
              MOV    p2,DH
              MOV    AH,4CH
              INT    21H
CODES ENDS
    END START


