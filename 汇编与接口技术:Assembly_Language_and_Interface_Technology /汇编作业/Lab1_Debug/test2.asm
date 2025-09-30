data segment
  buf1 db 'ABABCDEFGH'
  buf2 db 10 dup(?)
data ends

code segment
assume cs:code,ds:data
start: 
   mov ax,data
   mov ds,ax

  mov ax,0f365h
  mov cx,8100h
  add ax,cx   

exit: mov ah,4ch
      int 21h 

code ends
end start