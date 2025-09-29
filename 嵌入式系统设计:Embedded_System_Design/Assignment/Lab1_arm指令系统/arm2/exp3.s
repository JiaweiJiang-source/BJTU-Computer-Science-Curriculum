        AREA    example3, CODE, READONLY
        ENTRY

        ; ��ʼ�� r0 �� r1 ��ֵ���ֱ�Ϊ 15 �� 9
        MOV     r0, #15           
        MOV     r1, #9           

start
        CMP     r0, r1            
        BEQ     stop              ; ��� r0 == r1����ת�� stop�����Լ���� r0

        ; ��� r0 > r1
        BGT     greater_than      ; ��ת�� r0 > r1 �Ĵ�����

        ; ��� r0 < r1
less_than
        SUB     r1, r1, r0        ; r1 = r1 - r0
        B       start             

greater_than
        SUB     r0, r0, r1        ; r0 = r0 - r1
        B       start             

stop
        ; ���Լ���洢�� r0 ��
        B       stop              ; ����ѭ��������ֹͣ

        END
