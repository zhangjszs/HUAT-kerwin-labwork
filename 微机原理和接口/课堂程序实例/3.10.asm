
DATAS SEGMENT
     STRING1 DB 'INPUT char a!',13,10,'$'
     STRING2 DB 'INPUT other char!',13,10,'$'
DATAS ENDS
;�ж�������Ƿ����ַ�a
CODES SEGMENT
           ASSUME CS:CODES,DS:DATAS
     START:
           MOV    AX,DATAS
           MOV    DS,AX
     
           MOV    AH,01H
           INT    21H
     
           SUB    AL,'a'
           JE     LOAD1
           LEA    DX,STRING2
           JMP    SHOW
     LOAD1:
           LEA    DX,STRING1
     SHOW: 
           MOV    AH,9
           INT    21H
   
           MOV    AH,4CH
           INT    21H
CODES ENDS
    END   START






