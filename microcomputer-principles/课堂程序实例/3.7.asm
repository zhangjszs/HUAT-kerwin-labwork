; ------------------------------------------------------------------
; @Author: kerwin-win zhangjszs@foxmail.com
; @Date: 2024-04-02
; @LastEditors: kerwin-win
; @Description: 课堂示例 3.7（算术与进位、BCD/ASCII 调整、乘除等示例）
; @Notes: 保留原寄存器与数据定义
; ------------------------------------------------------------------
DATAS SEGMENT
      ; 数据段
      var0   db 0ABH
      var1   db 0A8H
      var2   db 0A9H
      var3   db 0AAH
      var4   db 0ABH
      var5   db 0AAH
      var6   db 036H
      var7   db 037H
      var8   db 038H
      var9   db 039H
      vard1  db 008H
      vard2  db 009H
      vard3  db 008H
      vard4  db 087H
      vard5  db 02EH
      varA   dw 00104H
      varB   dw 00607H
      varC   dw 034ABH
      varD   dw 023EFH
      varE   dw ?
      varDD1 dd 0A1A2A3A4H
      varDD2 dd 0B9B83936H
      varDD3 dd 0H

DATAS ENDS

STACKS SEGMENT
      ;�˴������ջ�δ���
STACKS ENDS

CODES SEGMENT
            ASSUME CS:CODES,DS:DATAS,SS:STACKS
      START:
            MOV    AX,DATAS
            MOV    DS,AX
      ;�˴��������δ���
            MOV    AL,var0
            ADD    AL,0AAH
            MOV    AH,var1
            ADD    AL,AH
            MOV    var0,AL
    
      ;
            MOV    AL,var1
            ADD    AL,var2
      ;ADC AH,0
            ADD    AL,var3
      ;ADC AH,0
            ADD    AL,var4
      ;ADC AH,0
            ADD    AL,var5
      ;ADC AH,0
      ;
            LEA    SI,varDD1
            LEA    DI,varDD2
            LEA    BX,varDD3
            CLC
            MOV    AL,[SI]
            ADC    AL,[DI]
            MOV    [BX],AL
            INC    SI
            INC    DI
            INC    BX
            MOV    AL,[SI]
            ADC    AL,[DI]
            MOV    [BX],AL
            INC    SI
            INC    DI
            INC    BX
            MOV    AL,[SI]
            ADC    AL,[DI]
            MOV    [BX],AL
            INC    SI
            INC    DI
            INC    BX
            MOV    AL,[SI]
            ADC    AL,[DI]
            MOV    [BX],AL
            INC    SI
            INC    DI
            INC    BX
      ;
            MOV    AL,var6
            MOV    BX,varA
            CBW
            ADD    AX,BX
    
      ;
            MOV    AL,vard1
            ADD    AL,vard2
            DAA
            ADD    AL,vard3
            DAA
            ADD    AL,vard4
            DAA
            MOV    AX,varA
            ADD    AX,varB
            AAA
    
      ;12*9
            MOV    AL,00CH
            MOV    BL,009H
      ;-12*8
            MOV    AL,11110100B
            MOV    BL,00001001B
      ;MOV BL
            IMUL   BL
      ;45/7
            MOV    AX, 0002DH
            MOV    BL, 07H
            DIV    BL
      ;301/(-11)
            MOV    AX, 0012DH
            MOV    BL, 11110101B
      ;DIV BL
            IDIV   BL
      ;6*4
            MOV    AL,036H
            MOV    BL,054H
            MUL    BL
            AAM
    
            MOV    AH,4CH
            INT    21H
CODES ENDS
    END START



