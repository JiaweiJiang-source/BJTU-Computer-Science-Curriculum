    area cdo,code,readonly  ;����cdo,�����
    code32          ;ARMָ��
gcd_asm
    GLOBAL  gcd_asm
    MOV     r2, r0     ; ����һ���������浽 r2
    MOV     r3, r1     ; ���ڶ����������浽 r3

start 
    CMP     r2, r3     ; �Ƚ� r2 �� r3
    BEQ     end        ; �����ȣ���ת�� end
    SUBLT   r3, r3, r2 ; r3 = r3 - r2 (r2 < r3)
    SUBGT   r2, r2, r3 ; r2 = r2 - r3 (r2 > r3)
    B       start      ; ����ѭ��

end
    MOV     r0, r2     ; ��������ص� r0
    BX      lr         ; ����

    END