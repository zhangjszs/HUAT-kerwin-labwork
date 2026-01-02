; ------------------------------------------------------------------
; @Author: kerwin-win zhangjszs@foxmail.com
; @Date: 2024-04-02
; @LastEditors: kerwin-win
; @Description: 实验 6.1（采集-回放示例，访问 ADC/DAC/8253/8255 设备端口）
; @Notes: 示例展示端口读写与延时子程序，保留原实现
; ------------------------------------------------------------------
DATA SEGMENT
       IMPORT  EQU 0E080H-280H
       IO0809A EQU IMPORT+29AH                                                         ;AD0809的地址
       IO0832A EQU IMPORT+290H                                                         ;DAC0832的入口地址
       IO8253A EQU IMPORT+280H                                                         ;8253的入口地址
       IO8253C EQU IMPORT+283H                                                         ;8253的控制字地址
       IO8255A EQU IMPORT+288H                                                         ;8255的入口地址
       IO8255C EQU IMPORT+28BH                                                         ;8255的控制字的地址
       MESG1   DB  0DH,0AH,'PRESS R TO RECORD!',0DH,0AH
               DB  'cs222 202202296 zhangchongwen',0DH,0AH,24H
       MESG2   DB  0DH,0AH,'PRESS P TO PLAY!', 0DH,0AH,24H
       MESG3   DB  0DH,0AH,'PRESS T TO CONTINUE,OTHER KEY TO EXIT!', 0DH,0AH,24H
       BUF     DB  60000  DUP(?)                                                       ;开辟内存空间存放数据
DATA ENDS
CODE SEGMENT
              ASSUME CS:CODE,DS:DATA
       START: MOV    AX,DATA               ;
              MOV    DS,AX                 ;
              MOV    DX,OFFSET MESG1       ;显示录音提示
              MOV    AH,09H                ;
              INT    21H
       TEST1: MOV    AH,1                  ;等待键盘输入
              INT    21H
              CMP    AL,'R'
              JNZ    TEST1                 ;与'R'比较，若是则执行下面的录音，不是则等待
              MOV    DI,OFFSET BUF         ;定义缓冲区指针
              MOV    CX,60000
       BEGIN1:MOV    DX, IO0809A           ;ADC0809的地址
              OUT    DX,AL                 ;启动一次A/D转换器
              CALL   DELAY                 ;调用延时子程序
              IN     AL,DX                 ;从端口读入数据
              MOV    [DI],AL               ;将数据放入缓冲区中
              INC    DI
              LOOP   BEGIN1                ;循环装入60000个数据
              MOV    DX,OFFSET MESG2       ;显示放音提示
              MOV    AH,9
              INT    21H
       TEST2: MOV    AH,1
              INT    21H
              CMP    AL,'P'
              JNZ    TEST2                 ;与'P'比较，若是则执行放音，不是则等待
       PLAY:  MOV    DI,OFFSET BUF         ;取缓冲区的入口地址
              MOV    CX,60000
       BEGIN2:MOV    DX,IO0832A            ;DAC0832的入口地址
              MOV    AL,[DI]               ;将要转换的数据从内存中取出
              OUT    DX,AL                 ;将数据从端口输出
              CALL   DELAY                 ;调用延时子程序
              INC    DI
              LOOP   BEGIN2
              MOV    DX,OFFSET MESG3       ;显示循环放音提示
              MOV    AH,9
              INT    21H
              MOV    AH,1
              INT    21H
              CMP    AL,'T'
              JZ     PLAY                  ;与'T'比较，若是则重新播放，不是则退出
              MOV    AH,4CH
              INT    21H
DELAY PROC NEAR
              PUSH   DX                    ;保存DX的内容
              MOV    DX,IO8253C            ;8253的控制字地址
              MOV    AL,00010000B          ;计数器0只读低字节以方式0工作，二进制计数
              OUT    DX,AL
              MOV    DX,IO8253A            ;8253的入口地址
              MOV    AL,200                ;写入循环计数初值
              OUT    DX,AL
              MOV    DX,IO8255C            ;8255的控制字的地址
              MOV    AL,10010000B          ;D7=1标志位,D6D5=00 方式0,D4=1表示A口输入
              OUT    DX,AL
              MOV    DX,IO8255A            ;8255的入口地址
       CHECK: IN     AL,DX                 ;将数据读入AL
              TEST   AL,01                 ;检查PA0的状态是否为高电平
              JZ     CHECK
              POP    DX                    ;恢复DX
              RET                          ;返回到DELAY入口
DELAY ENDP
CODE ENDS
END START