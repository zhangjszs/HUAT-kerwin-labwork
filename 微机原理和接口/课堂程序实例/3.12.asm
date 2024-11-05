
DATAS SEGMENT
     STRING1 DB 'INPUT LOWER CHAR!',13,10,'$'
     STRING2 DB 'INPUT OTHER char!',13,10,'$'
DATAS ENDS
;�ж�������Ƿ���Сд�ַ�
CODES SEGMENT
           ASSUME CS:CODES,DS:DATAS
     START:
           MOV    AX,DATAS
           MOV    DS,AX
     
           MOV    AH,01H
           INT    21H
     
           SUB    AL,'a'
           JL     LOAD2
           SUB    AL,'z'
           JNL    LOAD2
           LEA    DX,STRING1
           JMP    SHOW
     LOAD2:
           LEA    DX,STRING2
     SHOW: 
           MOV    AH,9
           INT    21H
   
           MOV    AH,4CH
           INT    21H
CODES ENDS
    END   START
