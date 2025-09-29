        AREA    example3, CODE, READONLY
        ENTRY

        ; 初始化 r0 和 r1 的值，分别为 15 和 9
        MOV     r0, #15           
        MOV     r1, #9           

start
        CMP     r0, r1            
        BEQ     stop              ; 如果 r0 == r1，跳转到 stop，最大公约数是 r0

        ; 如果 r0 > r1
        BGT     greater_than      ; 跳转到 r0 > r1 的处理部分

        ; 如果 r0 < r1
less_than
        SUB     r1, r1, r0        ; r1 = r1 - r0
        B       start             

greater_than
        SUB     r0, r0, r1        ; r0 = r0 - r1
        B       start             

stop
        ; 最大公约数存储在 r0 中
        B       stop              ; 无限循环，程序停止

        END
