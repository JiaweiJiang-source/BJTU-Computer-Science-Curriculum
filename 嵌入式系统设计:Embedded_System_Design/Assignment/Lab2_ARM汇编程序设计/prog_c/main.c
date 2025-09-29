//main.c
#define uint8	unsigned char
#define uint32	unsigned int

extern uint32	Add(uint32 x,uint32 y);

uint32 sum;

void MAIN(void)
{
	sum = Add(5,4);
	while(1);
}