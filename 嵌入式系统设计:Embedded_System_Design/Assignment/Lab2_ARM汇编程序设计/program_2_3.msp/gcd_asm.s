    area cdo,code,readonly  ;段名cdo,代码段
    code32          ;ARM指令
gcd_asm
    GLOBAL  gcd_asm
    MOV     r2, r0     ; 将第一个参数保存到 r2
    MOV     r3, r1     ; 将第二个参数保存到 r3

start 
    CMP     r2, r3     ; 比较 r2 和 r3
    BEQ     end        ; 如果相等，跳转到 end
    SUBLT   r3, r3, r2 ; r3 = r3 - r2 (r2 < r3)
    SUBGT   r2, r2, r3 ; r2 = r2 - r3 (r2 > r3)
    B       start      ; 继续循环

end
    MOV     r0, r2     ; 将结果返回到 r0
    BX      lr         ; 返回

    END