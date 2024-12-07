outport1        equ 2a0h
outport2        equ 2a8h
DATAS SEGMENT
            out DB' computer222 zhangchongwen 202202296 $'
DATAS ENDS

code segment
                 assume cs:code
      start:     

                 MOV    AX,DATAS
                 MOV    DS,AX
                 MOV    AH,9
                 LEA    DX,out
                 INT    21H
      MAIN:      
                 mov    dx,outport1
                 out    dx,al
                 call   delay            ;调延时子程序
                 mov    dx,outport2
                 out    dx,al
                 call   delay            ;调延时子程序
                 call   delay_long       ;新增延时5秒
                 jmp    MAIN
                 mov    ah,4ch
                 int    21h
delay proc near                          ;延时子程序
                 mov    bx,500
      lll:       mov    cx,0
      ll:                                ; loop ll  ; 移除无效循环
                 dec    bx
                 jne    lll
                 ret
delay endp

delay_long proc near                     ;新增延时子程序
                 mov    bx,1000          ;调整延时次数以实现5秒
      lll2:      call   delay
                 dec    bx
                 jne    lll2
                 ret
delay_long endp

code ends
end start