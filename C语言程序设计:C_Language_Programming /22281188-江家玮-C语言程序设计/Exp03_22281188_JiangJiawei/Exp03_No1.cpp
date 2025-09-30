#define _CRT_SECURE_NO_WARNINGS1
#include<stdio.h>
int main(void)
{
	int i = -3, k;
	float x = -2.58;

	k = (int)((int)x + (float)i);

	printf("k1 = %d\n", k);

	k = (float)(x = 85.58);
	printf("k2 = %d\n", k);
}