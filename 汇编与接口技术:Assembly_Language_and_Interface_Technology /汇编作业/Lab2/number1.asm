DATA SEGMENT
line_max_length               DB   0FFH
line                          DB   0, 100H  DUP(?)
letter_count                  DW   0
digit_count                   DW    0
other_count                   DW    0
input                         DB   'input a line:$'
output_letter_count           DB   0DH,0AH,'letter count:$'
output_digit_count            DB  0DH,0AH,'digit count:$'
output_other_count            DB  0DH,0AH,'other count:$'
DATA ENDS
 
STACK SEGMENT stack                         
DB  100H DUP(?)
STACK ENDS
 
CODE SEGMENT
	ASSUME CS:CODE,DS:DATA,SS:STACK
main   proc  far
start:
          PUSH DS
          MOV AX,0H
          PUSH AX
          MOV AX,DATA        
          MOV DS,AX
          LEA DX,input
	  MOV AH,9
	  INT 21H
	 
	  LEA DX,line_max_length
	  MOV AH,10 
	  INT 21H 
	   
	   MOV CL,line
	   XOR CH,CH
	   MOV SI,1
	   CMP CX,0         
	   JZ break          
       s:
           MOV AL,line[SI]
	   CMP AL,'0'             
	   JB other               
	   CMP AL,'9'             
	   JA letter_or_other    
	   INC digit_count       
	   JMP NEXT               
	letter_or_other:
	        AND AL,11011111b      
		CMP AL,'A'            
		JB other               
		CMP AL,'Z'           
		JA other               
		INC letter_count     
		JMP NEXT
	 other:
	        INC other_count         
	 next:
	       INC SI
	       LOOP s
     
    break:	 
       		LEA DX,output_letter_count
		MOV AH,9
		INT 21H	
		MOV AX,letter_count
		CALL decimal
		
		LEA DX,output_digit_count
		MOV AH,9
		INT 21H
		MOV AX,digit_count
		CALL decimal
		
		LEA DX,output_other_count
		MOV AH,9
		INT 21H
		MOV AX,other_count
		CALL decimal
       	ret
	   
main endp
 
decimal proc near 
 
       PUSH AX
		PUSH CX
		PUSH DX
		PUSH BX 
		
		CMP AX,0
		JGE no_negative 
		MOV BX,AX
		MOV DL,'-'
		MOV AH,2 
		INT 21H
		NEG BX 
		MOV AX,BX 
		
	no_negative:
	   MOV CX,0
	   MOV BX,10 
	  de:
	   XOR     DX,DX
	   DIV      BX
	   PUSH   DX 
	   INC      CX
	   CMP     AX,0 
	   JNZ      de 
	   
	  de1:
	   POP DX
	   ADD DL,30H
	   MOV AH,2 
	   INT 21H
	   LOOP de1 
	   
	  POP BX 
	  POP DX 
	  POP CX 
	  POP AX
	  
	  ret 
	  
decimal endp
CODE ENDS
END START