#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<math.h>

int main(void)
{
	int h;
	printf("请输入三角形的高度h:\n");
	scanf("%d", &h);
	for (int i = 0; i < 2 * h - 1; i++)
		printf("*");
	printf("\n");
	for (int i = 1; i < h; i++)
	{
		int temp = 0;
		for (int j = 0; j < i; j++)
		{
			printf(" ");
			temp = j + 1;
		}
		printf("*");
		for (int z = 1; z < 2 * h - temp * 2 - 2; z++)
			printf(" ");
		if(i != h -1)
			printf("*");
		printf("\n");
	}
}