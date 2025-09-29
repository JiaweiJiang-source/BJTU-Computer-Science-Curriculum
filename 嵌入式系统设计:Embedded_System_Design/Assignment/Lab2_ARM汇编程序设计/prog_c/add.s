	;add.s

	EXPORT	Add
	
	AREA	Addc,CODE,READONLY
	ENTRY
	CODE32
	
Add
	ADD		R0,R0,R1
	MOV		PC,LR
	
	END