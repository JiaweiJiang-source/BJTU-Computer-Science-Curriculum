#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<math.h>

int main(void)
{
	int n;
	long double sum = 0.0;
	int pre_son = 1;
	int pre_par = 1;
	printf("请输入正整数n:\n");
	scanf("%d", &n);
	for (int i = 0; i < n; i++)
	{
		int now_son = pre_par;
		int now_par = pre_par + pre_son;

		sum += (1.0 * now_par) / now_son;

		pre_par = now_par;
		pre_son = now_son;
		//printf("%d/%d+", pre_par, pre_son);
		//printf("%d/%d+", now_par, now_son);
	}
	printf("sum = %lf\n", sum);
}