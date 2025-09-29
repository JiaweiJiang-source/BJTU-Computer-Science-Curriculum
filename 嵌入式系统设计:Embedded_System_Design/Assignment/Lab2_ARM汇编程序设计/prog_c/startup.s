	;startup file
	IMPORT		|Image$$RO$$Limit|
	IMPORT		|Image$$RW$$Base|
	IMPORT		|Image$$ZI$$Base|
	IMPORT		|Image$$ZI$$Limit|
	
	IMPORT		MAIN		;declare the main() in the C Prog

	AREA	start,CODE,READONLY
	ENTRY
	CODE32
	
Reset
	LDR		SP,=0x40003f00
	
	;initial the C case
	
	LDR		R0,=|Image$$RO$$Limit|
	LDR		R1,=|Image$$RW$$Base|
	LDR		R3,=|Image$$ZI$$Base|
	
	CMP		R0,R1
	BEQ		LOOP1
	
LOOP0
	CMP		R1,R3
	LDRCC	R2,[R0],#4
	STRCC	R2,[R1],#4
	BCC		LOOP0
	
LOOP1
	LDR		R1,=|Image$$ZI$$Limit| 
	MOV		R2,#0
	
LOOP2
	CMP		R3,R1
	STRCC	R2,[R3],#4
	BCC		LOOP2
	
	B		MAIN
	END	