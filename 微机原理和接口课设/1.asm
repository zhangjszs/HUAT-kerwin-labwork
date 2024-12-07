ioport     equ  0e080h-280h
p8255a     equ  ioport+288h   ;8255    cs:288h~28fh
p8255b     equ  ioport+289h
p8255c     equ  ioport+28ah    
p8255ctl   equ  ioport+28bh
p8253_1    equ  ioport+281h    
p8253ctl   equ  ioport+283h
ad0809     equ  ioport+290h

;定义一个I/O端口操作的宏outport 
outport macro port,num
            mov dx,port    ;存放端口地址
            mov al,num     ;存放要输出的数据
            out dx,al      ;将要输出的数据输出到指定的端口
endm

;定义一个I/O端口操作的宏inport 
inport macro port,num
           mov dx,port    ;存放端口地址
           in  al,dx      ;将读取的数据存放到al寄存器中
endm

;定义一个字符串显示操作的宏strshow 
strshow macro string
            mov dx,offset string    ;将字符串的偏移地址存放到dx寄存器中
            mov ah,9                ;将9存储到ah寄存器中，ah用于存储功能号
            int 21h                 ;调用DOS的9号功能，将字符串显示在屏幕上
endm

data segment

    mess   db '     **********************************',0dh,0ah
           db '         project name',0dh,0ah
           db '         s:start',0dh,0ah
           db '         p:pause',0dh,0ah
           db '         Esc:recollect',0dh,0ah
           db '         q:exit',0dh,0ah
           db '         JSJ211 202102016 wang yu le',0dh,0ah
           db '         JSJ211 202102017 tan yue hang',0dh,0ah
           db '         JSJ211 202102021 zhnag jia yi',0dh,0ah
           db '     **********************************',0dh,0ah,'$'
    time_h db 0                                                        ;秒的百位
    time_l db 0                                                        ;秒的十位
    time_0 db 0                                                        ;秒的个位
    x_0    db 200                                                      ;倒计时时间控制变量
    ledb   db 3fh,06h,5bh,4fh,66h,6dh,7dh,07h,7fh,6fh
         
data ends

code segment
             assume  cs:code,ds:data
    start:   
             mov     ax,data
             mov     ds,ax
    ;strshow mess;菜单显示
             outport p8255ctl,81h       ;(10000001B)方式0，只C口低四位输入，AB以及C口高四位输出，8255初始化
             strshow mess               ;菜单显示

    ;倒计时时间设定
    again:   
             call    ad_kz              ;用于读取 AD 转换器的结果
	 
             call    led_show           ;用于显示数码管的内容

             mov     ah,0bh             ;功能号为 0Bh
             int     21h                ;用于等待用户从键盘输入一个字符
             cmp     al,0
             je      again              ;继续等待用户输入

             mov     ah,1               ;等待用户从键盘输入一个字符,将输入的字符存储在 AL 寄存器中
             int     21h                ;程序会暂停等待用户输入，并在用户按下键盘上的任意键后继续执行
             cmp     al,'q'
             je      exit               ;退出程序
             cmp     al,1bh             ;是Esc键吗？
             je      again
             cmp     al,'s'
             je      djs                ;执行相应的倒计时操作
             cmp     al,'p'
             je      pasue              ;执行相应的暂停操作
             jmp     again              ;继续等待用户输入
  
    ;倒计时暂停
    pasue:   
	  	
             call    led_show           ;用于显示数码管的内容
             mov     ah,0bh
             int     21h                ;等待用户从键盘输入一个字符
             cmp     al,0
             je      pasue              ; 继续等待用户输入
             mov     ah,1               ;等待用户从键盘输入一个字符,将输入的字符存储在 AL 寄存器中
             int     21h                ;程序会暂停等待用户输入，并在用户按下键盘上的任意键后继续执行
             cmp     al,'q'
             je      exit               ;退出程序
             cmp     al,1bh             ;是Esc键吗？
             je      again              ;继续等待用户输入
             cmp     al,'s'
             je      djs                ;执行相应的倒计时操作
             jmp     pasue              ;继续等待用户输入

    ;倒计时开始
    djs:     
    ;led_show子程序完成数码管显示，时间为1秒??????
             mov     cx,250             ;完成显示1秒
	  	
    L3:      
    ;由于其他子程序已经入栈保护，故此处不用将cx入栈
             call    led_show           ;用于显示数码管的内容
      		   
	

             LOOP    L3                 ; 继续执行剩余的循环次数

             dec     x_0                ;将会将存储在 x_0 变量所对应内存位置的值减一
             jz      again              ;倒计时时间已经为 0，跳转到标签 again，重新开始倒计时
             mov     ah,0bh
             int     21h                ;等待用户从键盘输入一个字符
             cmp     al,0
             je      djs                ;转到标签 djs，继续进行倒计时
             mov     ah,1               ;等待用户从键盘输入一个字符,将输入的字符存储在 AL 寄存器中
             int     21h                ;程序会暂停等待用户输入，并在用户按下键盘上的任意键后继续执行
             cmp     al,'q'
             je      exit               ;退出程序
             cmp     al,1bh             ;是Esc键吗？
             je      again
             cmp     al,'p'
             je      pasue
             jmp     djs
    exit:    
             outport p8255c,0           ;保证按q退出之后数码管全灭
             mov     ah,4ch             ;表示要执行的 DOS 功能是程序正常退出
             int     21h                ;调用 INT 21H 中断，DOS 将会终止当前运行的程序，并返回到操作系统

    ;process子程序的功能：
    ;将X中的3位十进制数的百位->对应的段码->time_h
    ;将X中的3位十进制数的十位->对应的段码->time_l
    ;将X中的3位十进制数的个位->对应的段码->time_0
    ;倒计时时间的各位数值被转换为对应的段码，并存储在相应的变量中，以便后续的数码管显示操作使用
process proc
             push    ax                 ;ax入栈保护
             push    bx                 ;bx入栈保护
             mov     al,x_0             ;将 x_0 的值移动到 al 寄存器中
             mov     ah,0               ;将 ah 寄存器清零，用于存储除法运算的余数
             mov     bl,100             ;将除数 100 移动到 bl 寄存器中
             div     bl                 ;将 ax 寄存器的值除以 bl 寄存器的值，商存储在 al 寄存器中，余数存储在 ah 寄存器中
             mov     bx,offset ledb     ;将 ledb 数组的偏移地址移动到 bx 寄存器中
             xlat                       ;根据 al 寄存器的值，从 bx 寄存器指向的内存位置读取数据，并将其移动到 al 寄存器中,用于将 x_0 的值映射到 ledb 数组中对应的段码
             mov     time_h,al          ;将 al 寄存器的值移动到 time_h 变量中，保存转换后的百位段码
             mov     al,ah              ;将 ah 寄存器的值移动到 al 寄存器中，准备进行下一次除法运算
             mov     ah,0               ;将 ah 寄存器清零，用于存储除法运算的余数
             mov     bl,10              ;将除数 10 移动到 bl 寄存器中
             div     bl                 ;将 ax 寄存器的值除以 bl 寄存器的值，商存储在 al 寄存器中，余数存储在 ah 寄存器中
             mov     bx,offset ledb     ;将 ledb 数组的偏移地址移动到 bx 寄存器中
             xlat                       ;将 x_0 的值映射到 ledb 数组中对应的段码
             mov     time_l,al          ;将 al 寄存器的值移动到 time_l 变量中，保存转换后的十位段码
             mov     al,ah              ;将 ah 寄存器的值移动到 al 寄存器中，准备进行下一次除法运算
             xlat                       ;用于将 x_0 的值映射到 ledb 数组中对应的段码
             mov     time_0,al          ;将 al 寄存器的值移动到 time_0 变量中，保存转换后的个位段码
             pop     bx                 ;bx调用结束出栈
             pop     ax                 ;ax调用结束出栈
             ret
      		
process endp

    ;led_show子程序的功能：数码管显示，直到1秒时间到
led_show proc
             push    ax                 ;将ax,cx,dx都入栈保护
             push    cx
             push    dx
             call    process
	 	
	 	
    ;pc4控制1号数码管显示语句
    L1:      

             call    delay
             outport p8255c,0           ;将 0 输出到 p8255c 端口，用于控制数码管的显示
             outport p8255a,time_0      ;pc4控制1号数码管显示语句,将 time_0 变量的值输出到 p8255a 端口，用于设置第一个数码管的显示内容
             outport p8255c,10h         ;将 20h（十六进制）输出到 p8255c 端口，用于控制数码管的显示
             call    delay
    ;pc5控制2号数码管显示语句
             outport p8255c,0
             outport p8255a,time_l
             outport p8255c,20h         ;将 40h（十六进制）输出到 p8255c 端口，用于控制数码管的显示
             call    delay
    ;pc6控制3号数码管显示语句
             outport p8255c,0
             outport p8255a,time_h
             outport p8255c,40h         ;将 80h（十六进制）输出到 p8255c 端口，用于控制数码管的显示

             call    delay

             pop     dx                 ;dx,cx,ax调用结束依次出栈
             pop     cx
             pop     ax
             ret
			
led_show endp


ad_kz proc
             push    ax                 ;将ax,bx,dx入栈保护
             push    bx
             push    dx
             outport ad0809, 0          ;启动 AD
    lop1:    inport  p8255c             ;获取 AD 转换器转换结束的状态。
    ;mov dx,port
    ;in al,dx
             test    al,01h             ;测试 al 寄存器的最低位是否为 1
             jz      lop1               ;如果 al 寄存器的最低位为 0，则跳转到 lop1 标签处，继续等待转换结束
             mov     dx,ad0809          ;存入端口地址
             in      al,dx              ;读取 AD 转换器的结果

             mov     bl,190             ;设定输入数值的上下线为200-10  ;190
             mul     bl                 ;将 al 寄存器的值与 bl 寄存器的值相乘。
             mov     bl,255             ;将 239 移动到 bl 寄存器。 ;255
             div     bl                 ;将 ax 寄存器的值除以 bl 寄存器的值，商存储在 al 寄存器中，余数存储在 ah 寄存器中
             adc     al,10              ;将 al 寄存器的值与 10 相加，用于将结果调整为 10-200 范围内的数值
             mov     x_0,al             ;将 al 寄存器的值移动到 x_0 变量中，保存转换后的数值
             pop     dx                 ;dx,bx,ax调用结束出栈
             pop     bx
             pop     ax
             ret
ad_kz endp

delay proc

             push    dx                 ;将dx,ax依次入栈保护
             push    ax
             outport p8253ctl,60h       ;送8253控制字，计数通道1，只读高八位，方式0
             mov     dx,p8253_1         ;将 p8253_0 端口地址移动到 dx 寄存器中？？？？
             mov     ax,3e7h            ;将 3e7h 999 的值移动到 ax 寄存器中，用于设置定时器的计数值
             out     dx,al              ;将 al 寄存器的值输出到 dx 寄存器指定的端口地址，用于设置定时器的计数值的低字节
             mov     al,ah              ;将 ah 寄存器的值移动到 al 寄存器中，准备设置定时器的计数值的高字节
             out     dx,al              ;将 al 寄存器的值输出到 dx 寄存器指定的端口地址，用于设置定时器的计数值的高字节
             mov     dx,p8255c          ;将 p8255c 端口地址移动到 dx 寄存器中
    w1:      in      al,dx              ;从 dx 端口读取数据，存储在 al 寄存器中
             test    al,04h             ;利用8253和8255配合实现延时
             jz      w1                 ;建议延时1---6ms(显示效果较好)
             pop     ax                 ;ax,dx出栈
             pop     dx                 ;将控制权返回到调用该过程的位置
             ret
			
delay endp

code ends
	 end start  
