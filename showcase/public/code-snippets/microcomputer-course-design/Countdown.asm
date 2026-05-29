; I/O端口地址按实际连接加以修改
ioport     equ  0e080h-280h          ; 计算基本I/O端口地址
p8255a     equ  ioport+288h          ; P8255A端口A的地址
p8255b     equ  ioport+289h          ; P8255B端口B的地址
p8255c     equ  ioport+28ah          ; P8255C端口C的地址
p8255ctl   equ  ioport+28bh          ; P8255控制寄存器的地址
p8253_0    equ  ioport+280h          ; 8253计数器0的地址
p8253ctl   equ  ioport+283h          ; 8253控制寄存器的地址
ad0809     equ  ioport+292h          ; AD0809模数转换器的地址

; 定义一个I/O端口操作的宏outport 
outport macro port,num        ; 输出数据到指定的I/O端口
            mov dx,port    ; 设置DX为端口地址
            mov al,num     ; 将要输出的数据放入AL
            out dx,al      ; 执行I/O写操作
endm

; 定义一个字符串显示操作的宏strshow 
strshow macro string         ; 显示字符串
            lea dx,string    ; 取字符串的偏移地址
            mov ah,9         ; DOS功能调用：打印字符串
            int 21h          ; 调用DOS中断服务
endm

data segment
    mess   db '     **********************************',0Dh,0Ah
           db '         Project: Countdown',0Dh,0Ah                    ; 项目名称
           db '     **********************************',0Dh,0Ah
           db '         s:start',0Dh,0Ah                               ; 用户提示信息
           db '         p:pause',0Dh,0Ah
           db '         Esc:cancel',0Dh,0Ah
           db '         q:exit',0Dh,0Ah
           db '     **********************************',0Dh,0Ah
           db '       Class CS222',0Dh,0Ah                             ; 程序员信息标题
           db '       Name: zcw, Number: 202202296',0Dh,0Ah
           db '       Name: ly, Number: 202202294',0Dh,0Ah
           db '       Name: wjx, Number: 202202298',0Dh,0Ah
           db '     **********************************',0Dh,0Ah,'$'
    time_h db 0                                                        ; 秒的百位存储位置
    time_l db 0                                                        ; 秒的十位存储位置
    time_0 db 0                                                        ; 秒的个位存储位置
    x      db 200                                                      ; 倒计时时间控制变量，初始值设为200秒
    ledb   db 3fh,6,5bh,4fh,66h,6dh,7dh,7,7fh,6fh                      ; 数码管0-9对应的段码
data ends

code segment
             assume  cs:code,ds:data
    start:   mov     ax,data            ; 初始化数据段寄存器
             mov     ds,ax
             strshow mess               ; 调用宏显示菜单信息
             outport p8255ctl,81h       ; 配置P8255工作模式（B0=1,A1A0=01）

    ; 倒计时时间设定
    again:   call    ad_kz              ; 调用子程序进行AD采样以设置倒计时时间
             call    led_show           ; 更新数码管显示当前时间
             mov     ah,0bh             ; 检查键盘缓冲区是否有键按下
             int     21h
             cmp     al,0               ; 如果没有按键则继续循环
             je      again
             mov     ah,1               ; 读取单个字符而不回显
             int     21h
             cmp     al,'q'             ; 如果用户输入'q'则退出程序
             je      exit
             cmp     al,1bh             ; 如果用户按下了Esc键，则返回主菜单
             je      again
             cmp     al,'s'             ; 如果用户输入's'则开始倒计时
             je      djs
             cmp     al,'p'             ; 如果用户输入'p'则暂停倒计时
             je      pasue
             jmp     again              ; 否则重新检查输入

    ; 倒计时暂停
    pasue:   call    led_show           ; 继续显示当前时间
             mov     ah,0bh             ; 检查键盘缓冲区
             int     21h
             cmp     al,0               ; 如果没有按键则继续等待
             je      pasue
             mov     ah,1               ; 读取单个字符
             int     21h
             cmp     al,'q'             ; 如果用户输入'q'则退出程序
