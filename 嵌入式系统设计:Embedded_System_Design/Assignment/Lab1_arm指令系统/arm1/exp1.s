         AREA  exp2,CODE,READONLY
         ENTRY
         CODE32      
START   LDR R4,=0x00090010      ;�洢�����ʵ�ַ
         LDR R13,=0x00090200         ;��ջ��ʼ��ַ
         MOV R0,#0x0F100                ;������Ѱַ
         MOV R2,#10
         MOV R1,R0                           ;�Ĵ���Ѱַ
         ADD R0,R1,R2
         STR R0,[R4]                         ;�Ĵ������
         LDR R3,[R4]
         MOV R0,R1, LSL#1             ;�Ĵ�����λ
         STR R0,[R4,#4]                   ;��ַ��ַ
         LDR R3,[R4,#4]!
         STMIA  R4!,{R0-R3}            ;��Ĵ���Ѱַ
         LDMIA  R4!,{R5,R6,R7,R8}
         STMFD R13!,{R5,R6,R7,R8}  ;��ջѰַ
         LDMFD R13!,{R1-R4}
         B START                                ;���Ѱַ
      
         END
