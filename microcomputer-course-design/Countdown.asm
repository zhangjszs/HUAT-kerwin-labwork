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
             je      exit
             cmp     al,1bh             ; 如果用户按下了Esc键，则返回主菜单
             je      again
             cmp     al,'s'             ; 如果用户输入's'则恢复倒计时
             je      djs
             jmp     pasue              ; 否则继续等待其他命令

    ; 倒计时开始
    djs:     call    led_show           ; 显示当前时间
             dec     x                  ; 减少倒计时变量x
             jz      again              ; 如果x等于0则跳转到again重新开始
             mov     ah,0bh             ; 检查键盘缓冲区
             int     21h
             cmp     al,0               ; 如果没有按键则继续倒计时
             je      djs
             mov     ah,1               ; 读取单个字符
             int     21h
             cmp     al,'q'             ; 如果用户输入'q'则退出程序
             je      exit
             cmp     al,1bh             ; 如果用户按下了Esc键，则返回主菜单
             je      again
             cmp     al,'p'             ; 如果用户输入'p'则暂停倒计时
             je      pasue
             jmp     djs                ; 否则继续倒计时

    exit:    outport p8255c,0           ; 关闭8255 PC端口
             mov     ah,4ch             ; DOS功能调用：终止程序并返回DOS
             int     21h

    ; process子程序的功能：
    ; 将X中的3位十进制数分解成百位、十位、个位，并转换为数码管所需的段码
process proc
             mov     al,x               ; 将倒计时变量x的值加载到AL中
             xor     ah,ah              ; 清除AH寄存器准备进行除法运算
             mov     bl,100
             div     bl                 ; AL = x / 100 (百位), AH = x % 100
             mov     bx,offset ledb     ; BX指向数码管段码表
             xlat                       ; AL = [BX + AL]，获取百位对应的段码
             mov     time_h,al          ; 存储百位段码
             mov     al,ah              ; AH中的余数现在是新的被除数
             xor     ah,ah              ; 再次清除AH寄存器
             mov     bl,10
             div     bl                 ; AL = (x % 100) / 10 (十位), AH = (x % 100) % 10 (个位)
             mov     bx,offset ledb     ; BX再次指向数码管段码表
             xlat                       ; AL = [BX + AL]，获取十位对应的段码
             mov     time_l,al          ; 存储十位段码
             mov     al,ah              ; AH中的余数即是个位
             xlat                       ; AL = [BX + AL]，获取个位对应的段码
             mov     time_0,al          ; 存储个位段码
             ret                        ; 返回
process endp

    ; led_show子程序的功能：数码管显示，直到1秒时间到
led_show proc
             call    process            ; 分解倒计时变量并转换为段码
             mov     cx,74              ; 循环次数，确保总延时约为1秒
    L1:                                 ; 动态扫描显示数码管
             outport p8255c,0           ; 复位PC端口
             outport p8255a,time_0      ; 发送个位段码到PA端口
             outport p8255c,10h         ; 选择1号数码管
             call    delay              ; 延时约4.5ms
             outport p8255c,0           ; 复位PC端口
             outport p8255a,time_l      ; 发送十位段码到PA端口
             outport p8255c,20h         ; 选择2号数码管
             call    delay              ; 延时约4.5ms
             outport p8255c,0           ; 复位PC端口
             outport p8255a,time_h      ; 发送百位段码到PA端口
             outport p8255c,40h         ; 选择3号数码管
             call    delay              ; 延时约4.5ms
             outport p8255c,0           ; 复位PC端口
             outport p8255a,3fh         ; 发送段码显示'0'到PA端口
             outport p8255c,80h         ; 选择4号数码管
             loop    L1                 ; 重复循环CX次
             ret                        ; 返回
led_show endp

ad_kz proc
             outport ad0809,0           ; 启动AD0809进行模数转换
    lop1:    mov     dx,p8255c          ; 获取状态端口地址
             in      al,dx              ; 从状态端口读取状态字
             test    al,01h             ; 测试PC0位是否为高电平表示转换结束
             jz      lop1               ; 如果转换未完成，继续等待
             mov     dx,ad0809          ; 获取AD结果端口地址
             in      al,dx              ; 读取AD转换结果
             mov     bl,190             ; 设定线性变换参数
             mul     bl                 ; AL = AL * BL
             mov     bl,255             ; 设定分母用于后续除法
             div     bl                 ; AL = (AL * 190) / 255
             add     al,10              ; 加上常数偏移量
             mov     x,al               ; 将计算后的值赋给倒计时变量x
             ret                        ; 返回
ad_kz endp

delay proc                              ; 实现约4.5ms的延时
             outport p8253ctl,20h       ; 设置8253定时器控制字
             mov     dx,p8253_0         ; 获取计数器0端口地址
             mov     ax,1194h           ; 设置计数值(对应约4.5ms)
             out     dx,al              ; 写入低字节
             mov     al,ah              ; 写入高字节
             out     dx,al
             mov     dx,p8255c          ; 获取状态端口地址
    w1:      in      al,dx              ; 读取状态字
             test    al,04h             ; 测试PC2位，等待定时器触发
             jz      w1                 ; 如果定时器未触发，继续等待
             ret                        ; 返回
delay endp

code ends
end start
