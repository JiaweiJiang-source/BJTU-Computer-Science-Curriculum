#define _CRT_SECURE_NO_WARNINGS 
#include<stdio.h>
#include<math.h>

int main(void)
{
	int n;
	long double sum = 1.0/2;
	long double temp = 1.0;
	long double ftemp = 1.0 / 2;
	scanf("%d", &n);

	for (int i = 0; i < n; i++)
	{
		if (i == 0)
			sum = 1.0/2;
		else
		{
			if (i != 0)
				temp *= (1.0 * (2 * i - 1) / (2 * i));
			ftemp = pow(ftemp, 2 * i + 1);
			sum += (temp * (1.0 / (2 * i + 1)) * ftemp);
			//printf("ftemp = %lf\n", ftemp);
		}
	}
	printf("Pi = %llf\n", 6*sum);
}