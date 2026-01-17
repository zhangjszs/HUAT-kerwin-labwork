; ------------------------------------------------------------------
; @Author: kerwin-win zhangjszs@foxmail.com
; @Date: 2024-04-02
; @LastEditors: kerwin-win
; @Description: 实验 5.1（7 段数码管交互示例，按键输入显示对应段码）
; @Notes: 使用 XLAT 查表并通过 8255 输出段码
; ------------------------------------------------------------------
DATA SEGMENT
      LED  DB 3FH,06H,5BH,4FH,66H,6DH,7DH,07H,7FH,6FH

      MSG  DB 0DH,0AH,'zhangchongwen 202202296 lizihang 202202297$',0DH,0AH,'$'
DATA ENDS

CODE SEGMENT
            ASSUME CS:CODE, DS:DATA
      START:
      ; 初始化数据段寄存器DS
            MOV    AX, DATA
            MOV    DS, AX
          
      ; 初始化8255芯片，假设8255的控制端口地址是28BH
            MOV    DX, 28BH
            MOV    AL, 80H
            OUT    DX, AL
   
      ; 输出个人信息
      ; 加载字符串MSG的地址到DX寄存器
            LEA    DX, MSG
          
      ; 设置功能号为09H，用于显示字符串
            MOV    AH, 09H
          
      ; 调用DOS中断21H，显示字符串
            INT    21H
      SSS:  
      ; 从键盘读取一个字符，结果存储在AL中
            MOV    AH, 01
            INT    21H
          
      ; 检查输入的字符是否小于'0'
            CMP    AL, '0'
            JL     EXIT
          
      ; 检查输入的字符是否大于'9'
            CMP    AL, '9'
            JG     EXIT
          
      ; 将字符转换为对应的数字（减去30H）
            SUB    AL, 30H
          
      ; 加载LED表的起始地址到BX寄存器
            LEA    BX, LED
          
      ; 使用AL中的数字作为索引，从LED表中获取对应的段码
            XLAT
      ; 从8255的A口输出段码
      ; 假设8255的A口地址是288H
            MOV    DX, 288H
            OUT    DX, AL
          
      ; 跳转回SSS标签，等待下一次输入
            JMP    SSS
      EXIT: 
      ; 程序结束，返回DOS
            MOV    AH, 4CH
            INT    21H
CODE ENDS
END START