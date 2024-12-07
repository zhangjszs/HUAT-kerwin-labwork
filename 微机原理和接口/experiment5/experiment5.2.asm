DATA SEGMENT
       ; 定义一个字节数组LED，存储0~9对应的7段数码管段码值
       LED     DB 3FH,06H,5BH,4FH,66H,6DH,7DH,07H,7FH,6FH
    
       ; 定义要打印的字符串MESSAGE，并以$结尾表示字符串结束
       MESSAGE DB 'zhangchongwen 202202296 lizihang 202202297 $',0
DATA ENDS

CODE SEGMENT
              ASSUME CS:CODE, DS:DATA

       START: 
       ; 初始化数据段寄存器
              MOV    AX, DATA
              MOV    DS, AX

       ; 打印字符串
       ; 将字符串MESSAGE的地址加载到DX寄存器
              LEA    DX, MESSAGE
           
       ; 设置AH为09H，准备打印字符串
              MOV    AH, 09H
           
       ; 调用DOS中断21H，打印字符串
              INT    21H

       ; 配置8255芯片
       ; 写控制字使8255的A口、C口为输出方式
              MOV    DX, 28BH
              MOV    AL, 80H
              OUT    DX, AL

       LOOP1: 
       ; 清除8255 C口
       ; 将AL设置为00H，清除C口
              MOV    AL, 00H
              MOV    DX, 28AH
              OUT    DX, AL

       ; 显示数字5
       ; 将AL设置为5
              MOV    AL, 5
           
       ; 将LED表的起始地址加载到BX寄存器
              MOV    BX, OFFSET LED
           
       ; 查表求出相应的段码给AL
              XLAT
           
       ; 将段码输出到8255的A口
              MOV    DX, 288H
              OUT    DX, AL

       ; 选择8255 C口位2
       ; 将AL设置为02H，选择C口位2
              MOV    AL, 02H
              MOV    DX, 28AH
              OUT    DX, AL

       ; 延时
       ; 设置CX为3000，进行延时
              MOV    CX, 3000
       DELAY: 
       ; 循环延时
              LOOP   DELAY

       ; 清除8255 C口
       ; 将AL设置为00H，清除C口
              MOV    AL, 00H
              MOV    DX, 28AH
              OUT    DX, AL

       ; 显示数字6
       ; 将AL设置为6
              MOV    AL, 6
           
       ; 将LED表的起始地址加载到BX寄存器
              MOV    BX, OFFSET LED
           
       ; 查表求出相应的段码给AL
              XLAT
           
       ; 将段码输出到8255的A口
              MOV    DX, 288H
              OUT    DX, AL

       ; 选择8255 C口位1
       ; 将AL设置为01H，选择C口位1
              MOV    AL, 01H
              MOV    DX, 28AH
              OUT    DX, AL

       ; 延时
       ; 设置CX为3000，进行延时
              MOV    CX, 3000
       DELAY1:
       ; 循环延时
              LOOP   DELAY1

       ; 检查键盘输入
       ; 检查是否有键盘输入
              MOV    AH, 01H
              INT    16H
           
       ; 如果有输入，跳转到EXIT
              JNZ    EXIT
           
       ; 如果没有输入，继续循环
              JMP    LOOP1

       EXIT:  
       ; 清除8255 A口
       ; 将AL设置为0H，清除A口
              MOV    AL, 0H
           
       ; 将0H输出到8255的A口
              MOV    DX, 288H
              OUT    DX, AL

       ; 返回DOS
       ; 设置AH为4CH，准备返回DOS
              MOV    AH, 4CH
           
       ; 调用DOS中断21H，结束程序
              INT    21H

CODE ENDS
END START