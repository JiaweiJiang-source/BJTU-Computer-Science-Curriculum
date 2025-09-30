#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
int main(void)
{
	int a, b;
	char c1, c2;
	double x, y, z;

	scanf("%d %d %c %c %lf %lf %lf", &a, &b, &c1, &c2, &x, &y, &z);
	printf("a = %d, b = %d, c1 = %c, c2 = %c, x = %.1lf, y = %.2lf, z = %.1lf\n", a, b, c1, c2, x, y, z);
	return 0;
}