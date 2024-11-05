; 程序功能：将键盘输入的两个5位十进制数相加并显示结果
; 要求：
; 1. 显示格式：被加数+加数=相加的结果
; 2. 考虑运算结果最高位有进位的情况
; 3. 使用子程序分别实现输入和显示功能

; ===== 宏定义部分 =====
strout macro string         ; 字符串输出宏
           mov ah,9         ; DOS 9号功能：显示字符串
           lea dx,string    ; 取字符串地址
           int 21h          ; 调用DOS中断
endm

c_show macro char         ; 单字符输出宏
           mov ah,2       ; DOS 2号功能：显示字符
           mov dl,char    ; 要显示的字符
           int 21h
endm

crlf macro             ; 回车换行宏
         mov ah,2
         mov dl,0dh    ; 回车符ASCII码
         int 21h
         mov ah,2
         mov dl,0ah    ; 换行符ASCII码
         int 21h
endm

; ===== 数据段定义 =====
data segment
    data1 db 5 dup(0)           ; 存放被加数的5个数字
          db '+','$'            ; 加号和结束符
    data2 db 5 dup(0)           ; 存放加数的5个数字
          db '=','$'            ; 等号和结束符
    data3 db 5 dup(0)           ; 存放计算结果
          db '$'                ; 字符串结束符
    mess1 db 'input data1:$'    ; 第一个数输入提示
    mess2 db 'input data2:$'    ; 第二个数输入提示
data ends

; ===== 代码段 =====
code segment
           assume cs:code,ds:data

    start: 
           mov    ax,data            ; 初始化数据段
           mov    ds,ax
           call   input              ; 调用输入子程序
           call   output             ; 调用输出和计算子程序
           mov    ax,4c00h           ; 程序正常结束
           int    21h

    ; ===== 输入子程序 =====
input proc
           strout mess1              ; 显示第一个数输入提示
           mov    cx,5               ; 设置循环计数为5
           lea    si,data1           ; SI指向被加数存储区
    x1:    
           mov    ah,1               ; DOS 1号功能：键盘输入
           int    21h
           mov    [si],al            ; 存储输入的数字
           inc    si                 ; 指向下一个存储位置
           loop   x1

           crlf                      ; 换行
           strout mess2              ; 显示第二个数输入提示
    
           mov    cx,5               ; 重置循环计数
           lea    si,data2           ; SI指向加数存储区
    x2:    
           mov    ah,1               ; 键盘输入
           int    21h
           mov    [si],al            ; 存储输入的数字
           inc    si
           loop   x2
    
           crlf                      ; 换行
           ret
input endp

    ; ===== 输出和计算子程序 =====
output proc
           strout data1              ; 显示被加数和加号
           strout data2              ; 显示加数和等号

           mov    cx,5               ; 设置5位数字的循环计数
           lea    si,data1+4         ; SI指向被加数最低位
           lea    di,data2+4         ; DI指向加数最低位
           lea    bx,data3+4         ; BX指向结果最低位
           clc                       ; 清除进位标志

    x3:    
           mov    al,[si]            ; 取被加数一位
           adc    al,[di]            ; 加上加数对应位(带进位)
           aaa                       ; ASCII调整
           pushf                     ; 保存标志位
           add    al,30h             ; 转换为ASCII码
           popf                      ; 恢复标志位
           mov    [bx],al            ; 存储结果
           dec    si                 ; 移动指针到高一位
           dec    di
           dec    bx
           loop   x3

           jnc    x4                 ; 如果最高位无进位，跳转
           mov    ah,2               ; 有进位，显示'1'
           mov    dl,'1'
           int    21h

    x4:    
           strout data3              ; 显示最终结果
           ret
output endp

code ends
end start