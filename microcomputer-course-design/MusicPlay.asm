 ;点歌系统：
;输入不同的数字，播放相应的音乐。
;其中“1”代表《刚好遇见你》音乐,“2”代表《成都》音乐,“3”代表《暧昧》音乐。
;输入数字“0”，则退出点歌系统。
;----------------------------------------------------
stack segment para stack 'stack'
          db 100 dup ('?')
stack ends
;----------------------------------------------------
data segment para 'data'
    tip0      db '-----------------------------------','$'
    tip1      db '|  1:    two_tigers               |','$'
    tip2      db '|  2:    star                     |','$'
    tip3      db '|  3:    songbie                  |','$'
    tip4      db '|  0: exit                        |','$'
    tip5      db '  --please input your choice:      ','$'
    tip       dw tip0,tip1,tip2,tip3,tip4,tip0,tip5           ;界面
    tip6      db 'music over!','$'
    tip7      db 'begin:','$'
    tip8      db 'end  :','$'
    tip9      db 'You have exited successfully.','$'
    tip10     db 'input error!please input again:','$'
    time      db 'yy/mm/dd hh:mm:ss','$'                      ;输出时间的格式
    table     db 9,8,7,4,2,0                                  ;CMOS ROM中时间信息的存放单元

    ;----------------------------------------------------
    ;《两只老虎》
    mus_freq1 dw 262,294,330,262                              ;
              dw 262,294,330,262                              ;
              dw 330,349,392                                  ;
              dw 330,349,392                                  ;
              dw 392,440,392,349,330,262                      ;
              dw 392,440,392,349,330,262                      ;
              dw 294,196,262                                  ;
              dw 294,196,262 ,-1                              ;
          
    mus_time1 dw 50,50,50,50                                  ;
              dw 50,50,50,50                                  ;
              dw 50,50,100                                    ;
              dw 50,50,100                                    ;
              dw 25,25,25,25,50,50                            ;
              dw 25,25,25,25,50,50                            ;
              dw 50,50,100                                    ;
              dw 50,50,100                                    ;
    ;---------------------------------------------------
    ;《一闪一闪亮晶晶》
    mus_freq2 dw 262,262,392,392
              dw 440,440,392
              dw 349,349,330,330
              dw 294,294,162
              dw 392,392,349,349
              dw 330,330,294
              dw 392,392,349,349
              dw 330,330,294
              dw 262,262,392,392
              dw 440,440,392,-1
           
    mus_time2 dw 25,25,25,25
              dw 25,25,50
              dw 25,25,25,25
              dw 25,25,50
              dw 25,25,25,25
              dw 25,25,50
              dw 25,25,25,25
              dw 25,25,50
              dw 25,25,25,25
              dw 25,25,50
           
    ;-------------------------------------------------
    ;《送别》
    mus_freq3 dw 392,330,392,524
              dw 440,524,392
              dw 392,262,294,330,294,262,294
              dw 392,330,392,524,494
              dw 440,524,392
              dw 392,294,330,349,247
              dw 262
           
              dw 440,524,524
              dw 494,440,494,524
              dw 440,494,524,440,440,392,330,262
              dw 294
              dw 392,330,392,524,494
              dw 440,524,392
              dw 392,294,330,349,247
              dw 262
              dw -1
    mus_time3 dw 50,25,25,100
              dw 50,50,100
              dw 50,25,25,50,25,25,150
              dw 50,25,25,50,25
              dw 50,50,100
              dw 50,25,25,50,50
              dw 100
           
              dw 50,50,100
              dw 50,25,25,100
              dw 25,25,25,25,25,25,25,25
              dw 150
              dw 50,25,25,50,25
              dw 50,50,100
              dw 50,25,25,50,50
              dw 150

    ;---------------------------------------------------
    ;直接定址表
    mus_time  dw mus_time1,mus_time2,mus_time3
    mus_freq  dw mus_freq1,mus_freq2,mus_freq3
data ends
;----------------------------------------------------
code segment para 'code'
               assume cs:code,ss:stack,ds:data
music proc far
               mov    ax, data
               mov    ds, ax
        
               call   tips                             ; 打印界面

    start_loop:                                        ; 定义循环起点
		
               mov    ax, data
               mov    ds, ax
        
               call   tips                             ; 打印界面
		
               mov    bh, 00                           ; 第0页
               mov    dh, 06                           ; 第7行
               mov    dl, 42                           ; 第42列
               mov    ah, 02
               int    10h                              ; 设置光标位置

               mov    ah, 01
               int    21h                              ; 键盘输入并回显

		
    input:     cmp    al,30h                           ;和'0'比较
               jb     end_error                        ;调用end_error处理错误输入
               cmp    al,33h                           ;和'3'比较
               ja     end_error                        ;调用end_error处理错误输入
               cmp    al,30h                           ;判断输入是否为'0'
               je     end_exit                         ;调用end_exit退出程序
		
               sub    al,30h                           ;ASCII码转换为对应数字
               dec    al                               ;减一，因为地址从0开始
               mov    ah,0                             ;ax寄存器高位置0
               shl    ax,1                             ;每个地址数据占两个字节，向左移位实现*2
               mov    di,ax                            ;相对位移赋值给变地寄存器实现寄存器相对寻址
               mov    si,mus_freq[di]
               mov    bp,mus_time[di]
		
		
               mov    ah,02
               mov    dh,08
               mov    dl,20
               int    10h                              ;设置光标位置
		
               mov    ah,09
               lea    dx,tip7
               int    21h                              ;显示输出
		
               push   dx
               mov    dh,08
               mov    dl,26
               call   timer                            ;调用timer子程序显示开始播放时间，dh设置显示行数，dl设置显示列数
               pop    dx
		
    freq:      
               mov    di,[si]
               cmp    di,-1
               je     end_mus                          ;遇到-1时中止乐曲
               mov    bx,ds:[bp]
               call   soundf                           ;调用发声子程序
               add    si,2
               add    bp,2
               jmp    freq

    end_mus:   
               mov    bh, 00
               mov    dh, 10
               mov    dl, 26
               mov    ah, 02
               int    10h                              ; 设置光标位置

               mov    ah, 09
               lea    dx, tip6
               int    21h                              ; 显示输出 "music over!"

               mov    ah, 02
               mov    dh, 11
               mov    dl, 20
               int    10h                              ; 设置光标位置

               mov    ah, 09
               lea    dx, tip8
               int    21h                              ; 显示输出 "end :"

               push   dx
               mov    dh, 11
               mov    dl, 26
               call   timer                            ; 调用 timer 程序显示结束时间
               pop    dx

    ; 清屏命令
               mov    ax, 0600h                        ; 功能号 06h (清屏)，AL = 0 (滚动所有行)
               mov    bh, 07                           ; 属性：白色前景，黑色背景
               mov    cx, 0                            ; 左上角坐标 (CH = 行, CL = 列)
               mov    dx, 184Fh                        ; 右下角坐标 (DH = 行, DL = 列)；对于80x25文本模式，这里是最后一行最后一列
               int    10h                              ; 执行 BIOS 视频服务

               call   tips                             ; 重新打印界面

               jmp    start_loop                       ; 修改为跳转回 music 过程的开始，以保持循环

    end_exit:  
               mov    bh,00
               mov    dh,09
               mov    dl,20
               mov    ah,02
               int    10h                              ;设置光标位置
		
               mov    ah,09
               lea    dx,tip9
               int    21h                              ;显示输出
		
               jmp    music_end
		
    end_error: 
               mov    ax,0b800h
               mov    es,ax
               mov    si,7*160+44*2
               mov    byte ptr es:[si],20h             ;将输入处清空，以处理第二次仍然是错误的情况

               mov    bh,00
               mov    dh,07
               mov    dl,12
               mov    ah,02
               int    10h                              ;设置光标位置
		
               mov    ah,09
               lea    dx,tip10
               int    21h                              ;显示输出
		
               mov    bh,00                            ;第0页
               mov    dh,07                            ;第7行
               mov    dl,44                            ;第44列
               mov    ah,02
               int    10h                              ;设置光标位置
        
               mov    ah,01
               int    21h                              ;键盘输入并回显
		
               jmp    input
		
    music_end: 
               mov    ax,4c00h
               int    21h
music endp
    ;-----------------------------------------------------
    ;发声程序
soundf proc far
               push   ax
               push   bx
               push   cx
               push   dx
               push   di
        
    ; 初始化定时器2
               mov    al,0b6h
               out    43h,al
               mov    dx,12h
               mov    ax,348Ch
               div    di                               ; 通过频率计算声音计数值
               out    42h,al                           ; 发送低8位
               mov    al,ah
               out    42h,al                           ; 发送高8位
               in     al,61h
               mov    ah,al
               or     al,3
               out    61h,al                           ; 开启扬声器

    WAIT1:     mov    cx,4971                          ; 控制音符的持续时间
               call   waitf
               dec    bx
               jnz    WAIT1

    ; 关闭扬声器
               mov    al,ah
               out    61h,al

    ; *** 增加音符间的停顿 ***
               mov    cx,8000                          ; 调整CX的值来控制停顿时间
               call   waitf                            ; 调用延迟子程序

               pop    di
               pop    dx
               pop    cx
               pop    bx
               pop    ax
               ret
soundf endp

    ;----------------------------------------------------
    ;时间延迟
waitf proc far
               push   ax
    waitf1:    
               in     al, 61h                          ; 从端口 61h 读取状态，存入 al
               and    al, 10h                          ; 取 al 的第 4 位，检测特定的标志位（通常与硬件定时器状态相关）
               cmp    al, ah                           ; 比较 al 和 ah 的值
               je     waitf1                           ; 如果 al == ah，表示状态未变化，继续等待
               mov    ah, al                           ; 如果状态变化了，将 al 的值存入 ah
               loop   waitf1                           ; 继续循环，直到 cx 为 0
               pop    ax
               ret
waitf endp

    ;------------------------------------------------------
    ;打印界面 按预先设定的tip中的内容输出并更改背景颜色
tips proc far
               push   ax
               push   ds
               push   si
               push   cx
	
               mov    si,offset tip
               sub    si,2
               mov    cx,7
               mov    al,-1
    tipsf:     

               mov    bh,0
               inc    al
               mov    dh,al
               mov    dl,12
               mov    ah,2
               int    10h
		
               push   ax
               add    si,2
               mov    dx,[si]
               mov    ah,09
               int    21h
               pop    ax


               loop   tipsf
		
               mov    ax,0b800h
               mov    es,ax
               mov    bp,0
               mov    si,25
    colorf:                                            ;改背景颜色
               mov    byte ptr es:[bp+si],01110000B    ;颜色属性
               add    si,2
               cmp    si,95                            ;控制改色区域的列的范围
               jb     colorf
               add    bp,160
               mov    si,25
               cmp    bp,160*6                         ;控制改色区域为1-6行
               jb     colorf
		
               pop    cx
               pop    si
               pop    ds
               pop    ax
               ret
		
tips endp
    ;------------------------------------------------------
    ;显示当前时间，dh设置输出所在的行数，dl设置列数
timer proc far
               push   ax
               push   ds
               push   si
               push   di
               push   cx
               push   bx
	
               mov    ax,data
               mov    ds,ax
    start:     mov    si,offset table
               mov    di,offset time
		
               mov    cx,6
    s1:        push   cx
               mov    al,ds:[si]
               out    70h,al                           ;向70h写入要访问的单元的地址
               in     al,71h                           ;从71h中得到指定单元的数据
		
               mov    ah,al
               mov    cl,4
               shr    ah,cl                            ;十位数码值
               add    ah,30h                           ;转换位ASCII码
               and    al,00001111b                     ;个位数码值
               add    al,30h                           ;转换位ASCII码
               mov    ds:[di],ah
               mov    ds:[di+1],al
               add    di,3
               inc    si
               pop    cx
               loop   s1
		
               mov    bh,0
               mov    ah,2
               int    10h                              ;根据预先指定的dh，dl设定光标位置
		
               mov    dx,offset time
               mov    ah,9
               int    21h                              ;显示输出
		
               pop    bx
               pop    cx
               pop    di
               pop    si
               pop    ds
               pop    ax
               ret
timer endp
    ;------------------------------------------------------
code ends
;---------------------------------------------------------
        end     music 

11b;个位数码值
		add al,30h;转换位ASCII码
		mov ds:[di],ah
		mov ds:[di+1],al
		add di,3
		inc si
		pop cx		
		loop s1
		
		mov bh,0
		mov ah,2
		int 10h;根据预先指定的dh，dl设定光标位置
		
		mov dx,offset time
		mov ah,9
		int 21h;显示输出
		
		pop bx
		pop cx
		pop di
		pop si
		pop ds
		pop ax
		ret
timer endp
;------------------------------------------------------
code    ends
;---------------------------------------------------------
        end     music 








