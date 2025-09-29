         AREA  exp2,CODE,READONLY
         ENTRY
         CODE32      
START   LDR R4,=0x00090010      ;´æ´¢Æ÷·ÃÎÊµØÖ·
         LDR R13,=0x00090200         ;¶ÑÕ»³õÊ¼µØÖ·
         MOV R0,#0x0F100                ;Á¢¼´ÊýÑ°Ö·
         MOV R2,#10
         MOV R1,R0                           ;¼Ä´æÆ÷Ñ°Ö·
         ADD R0,R1,R2
         STR R0,[R4]                         ;¼Ä´æÆ÷¼ä½Ó
         LDR R3,[R4]
         MOV R0,R1, LSL#1             ;¼Ä´æÆ÷ÒÆÎ»
         STR R0,[R4,#4]                   ;»ùÖ·±äÖ·
         LDR R3,[R4,#4]!
         STMIA  R4!,{R0-R3}            ;¶à¼Ä´æÆ÷Ñ°Ö·
         LDMIA  R4!,{R5,R6,R7,R8}
         STMFD R13!,{R5,R6,R7,R8}  ;¶ÑÕ»Ñ°Ö·
         LDMFD R13!,{R1-R4}
         B START                                ;Ïà¶ÔÑ°Ö·
      
         END
