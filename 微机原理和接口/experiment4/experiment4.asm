DATA SEGMENT
    MSG1  DB 0DH,0AH,'THIS IS A 8259A INTERRUPT',0DH,0AH,'$'                     ; 定义字符串MSG1
    MSG2  DB 0DH,0AH,'PRESS ANY KEY TO EXIT!',0DH,0AH,'$'                        ; 定义字符串MSG2
    MSG3  DB 0DH,0AH,'zhangchongwen 202202296 lizihang 202202297',0DH,0AH,'$'    ; 定义字符串MSG3
    CSREG DW ?                                                                   ; 旧中断向量保存空间
    IPREG DW ?                                                                   ; 旧中断向量保存空间
DATA ENDS

CODE SEGMENT
             ASSUME CS:CODE, DS:DATA, ES:DATA

    START:   
             MOV    AX, DATA                     ; 将数据段的段地址加载到AX寄存器
             MOV    DS, AX                       ; 将数据段的段地址加载到DS寄存器
             MOV    ES, AX                       ; 将数据段的段地址加载到ES寄存器
             CLI                                 ; 关中断

    ; 打印初始字符串
             MOV    DX, OFFSET MSG3              ; 将MSG3字符串的偏移地址加载到DX寄存器
             MOV    AH, 09H                      ; 设置AH寄存器为09H，表示调用DOS的9号功能（显示字符串）
             INT    21H                          ; 调用DOS中断，显示MSG3字符串

    ; 保存原中断向量
             MOV    AX, 350BH                    ; 设置AX寄存器为350BH，表示调用DOS的35H号功能（获取中断向量），中断类型号为0BH
             INT    21H                          ; 调用DOS中断，获取中断向量，结果保存在ES:BX中
             MOV    AX, ES                       ; 将中断向量的段地址保存到AX寄存器
             MOV    CSREG, AX                    ; 将中断向量的段地址保存到CSREG
             MOV    IPREG, BX                    ; 将中断向量的偏移地址保存到IPREG

    ; 设置新中断向量
             MOV    AX, CS                       ; 将代码段的段地址加载到AX寄存器
             MOV    DS, AX                       ; 将代码段的段地址加载到DS寄存器
             MOV    DX, OFFSET INT_PROC          ; 将中断服务程序INT_PROC的偏移地址加载到DX寄存器
             MOV    AX, 250BH                    ; 设置AX寄存器为250BH，表示调用DOS的25H号功能（设置中断向量），中断类型号为0BH
             INT    21H                          ; 调用DOS中断，设置新的中断向量

    ; 设置中断掩码
             IN     AL, 21H                      ; 从端口21H读取中断屏蔽寄存器的值到AL寄存器
             AND    AL, 11110111B                ; 清除AL寄存器的第3位（IRQ3），开放IRQ3中断
             OUT    21H, AL                      ; 将修改后的中断屏蔽寄存器的值写回到端口21H

    ; 实现主要功能段
             MOV    AX, DATA                     ; 重新设置数据段的段地址
             MOV    DS, AX                       ; 将数据段的段地址加载到DS寄存器
             MOV    DX, OFFSET MSG2              ; 将MSG2字符串的偏移地址加载到DX寄存器
             MOV    AH, 09H                      ; 设置AH寄存器为09H，表示调用DOS的9号功能（显示字符串）
             INT    21H                          ; 显示MSG2字符串
             STI                                 ; 开中断，允许中断发生
             CALL   INIT8253                     ; 调用INIT8253子程序，初始化8253定时器

    LOOP1:   
             MOV    AH, 1                        ; 设置AH寄存器为1，表示调用BIOS的1号功能（检测键盘是否有键入的字符）
             INT    16H                          ; 调用BIOS中断，检测键盘输入
             JE     LOOP1                        ; 如果ZF标志位为1（表示无键按下），则跳转到LOOP1继续循环

    EXIT:    
             CLI                                 ; 关中断
    
    ; 恢复原中断向量
             MOV    DX, IPREG                    ; 将旧的中断向量的偏移地址加载到DX寄存器
             MOV    AX, CSREG                    ; 将旧的中断向量的段地址加载到AX寄存器
             MOV    DS, AX                       ; 将旧的中断向量的段地址加载到DS寄存器
             MOV    AX, 250BH                    ; 设置AX寄存器为250BH，表示调用DOS的25H号功能（设置中断向量），中断类型号为0BH
             INT    21H                          ; 调用DOS中断，恢复旧的中断向量

             MOV    AX, 4C00H                    ; 设置AX寄存器为4C00H，表示调用DOS的4CH号功能（返回DOS）
             INT    21H                          ; 调用DOS中断，返回DOS

    ; 恢复中断掩码
             IN     AL, 21H                      ; 从端口21H读取中断屏蔽寄存器的值到AL寄存器
             OR     AL, 00001000B                ; 设置AL寄存器的第3位（IRQ3），关闭IRQ3中断
             OUT    21H, AL                      ; 将修改后的中断屏蔽寄存器的值写回到端口21H

    ; 中断服务程序
INT_PROC PROC FAR
             CLI                                 ; 关中断
             PUSH   AX                           ; 保护寄存器
             PUSH   DX
             PUSH   DS
             MOV    AX, DATA                     ; 重新设置数据段的段地址
             MOV    DS, AX                       ; 将数据段的段地址加载到DS寄存器
             MOV    DX, OFFSET MSG1              ; 将MSG1字符串的偏移地址加载到DX寄存器
             MOV    AH, 09H                      ; 设置AH寄存器为09H，表示调用DOS的9号功能（显示字符串）
             INT    21H                          ; 显示MSG1字符串
             MOV    AL, 20H                      ; 设置AL寄存器为20H，表示发出EOI结束中断
             OUT    20H, AL                      ; 向主8259A发送EOI信号
             POP    DS                           ; 恢复寄存器
             POP    DX
             POP    AX
             STI                                 ; 开中断
             IRET                                ; 从中断服务程序返回
INT_PROC ENDP

    ; 初始化8253定时器
INIT8253 PROC
             MOV    DX, 293H                     ; 将控制口地址293H加载到DX寄存器
             MOV    AL, 36H                      ; 设置AL寄存器为36H，使0通道为工作方式3
             OUT    DX, AL                       ; 将控制字写入8253的控制寄存器
             MOV    AX, 2000                     ; 设置AX寄存器为2000，写入循环计数初值2000
             MOV    DX, 290H                     ; 将0通道地址290H加载到DX寄存器
             OUT    DX, AL                       ; 先写入低字节
             MOV    AL, AH                       ; 将高字节加载到AL寄存器
             OUT    DX, AL                       ; 后写入高字节
             MOV    DX, 293H                     ; 将控制口地址293H加载到DX寄存器
             MOV    AL, 76H                      ; 设置AL寄存器为76H，设8253通道1工作方式2
             OUT    DX, AL                       ; 将控制字写入8253的控制寄存器
             MOV    AX, 2000                     ; 设置AX寄存器为2000，写入循环计数初值2000
             MOV    DX, 291H                     ; 将1通道地址291H加载到DX寄存器
             OUT    DX, AL                       ; 先写低字节
             MOV    AL, AH                       ; 将高字节加载到AL寄存器
             OUT    DX, AL                       ; 后写高字节
             RET                                 ; 从子程序返回
INIT8253 ENDP

CODE ENDS
END START