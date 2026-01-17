; 编程实现：从键盘分别输入两个字符串，然后进行比较，若两个字符串的长度和对应字符都完全相同，则显示“MATCH”，否则显示“NO MATCH”。
data segment
    ; 提示信息
    info1   db 'please input first string',0ah,'$'
    info2   db 0ah,'please input second string',0ah,'$'

    ; 字符串缓冲区
    string1 db 101,?,101 dup (?)                           ; 存储第一个字符串，前一个字节存储最大长度
    string2 db 101,?,101 dup (?)                           ; 存储第二个字符串，前一个字节存储最大长度

    ; 匹配和不匹配的消息
    match   db 0ah,'match',0ah,'$'
    nomatch db 0ah,'no match',0ah,'$'
data ends

stack segment stack
          dw  20h dup(?)    ; 定义栈空间
          top label word    ; 栈顶标签
stack ends

code segment
         assume ds:data,cs:code,ss:stack

p proc far

    ; 初始化数据段
         mov    ax,data
         mov    ds,ax

    ; 初始化栈段
         mov    ax,stack
         mov    ss,ax
         lea    sp,top

    ;; 获取第一个字符串输入
         lea    dx,info1                    ; 加载提示信息
         mov    ah,09h                      ; DOS打印字符串功能
         int    21h                         ; 调用DOS中断

         lea    dx,string1                  ; 加载字符串缓冲区
         mov    ah,0ah                      ; DOS输入字符串功能
         int    21h                         ; 调用DOS中断

    ;; 获取第二个字符串输入
         lea    dx,info2                    ; 加载提示信息
         mov    ah,09h                      ; DOS打印字符串功能
         int    21h                         ; 调用DOS中断

         lea    dx,string2                  ; 加载字符串缓冲区
         mov    ah,0ah                      ; DOS输入字符串功能
         int    21h                         ; 调用DOS中断

    ;; 比较字符串长度
         mov    al,string1+1                ; 读取第一个字符串的长度
         mov    bl,string2+1                ; 读取第二个字符串的长度
         cmp    al,bl                       ; 比较两个长度
         jne    no                          ; 如果不相等，跳转到no标签

    ; 字符串长度相等，继续比较内容
         mov    cl,al                       ; 保存字符串长度
         lea    si,string1+2                ; SI指向第一个字符
         lea    di,string2+2                ; DI指向第一个字符

    l1:  
         mov    al,[si]                     ; 加载第一个字符串的字符
         mov    bl,[di]                     ; 加载第二个字符串的字符
         cmp    al,bl                       ; 比较字符
         jne    no                          ; 如果不相等，跳转到no标签

    ; 继续检查下一个字符
         inc    si                          ; SI指向下一个字符
         inc    di                          ; DI指向下一个字符
         dec    cl                          ; 减少字符计数
         jnz    l1                          ; 如果计数不为零，继续循环

    yes: 
    ; 输出匹配消息
         lea    dx,match
         mov    ah,09h
         int    21h
         jmp    exit                        ; 跳转到exit标签

    no:  
    ; 输出不匹配消息
         lea    dx,nomatch
         mov    ah,09h
         int    21h

    exit:
    ; 退出程序
         mov    ah,4ch
         int    21h

p endp
code ends
    end p
