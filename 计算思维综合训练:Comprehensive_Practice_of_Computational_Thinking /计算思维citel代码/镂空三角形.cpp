#include<stdio.h>
int main(void)
{
	char c;
	int n, sum = 1;
	c = getchar();
	scanf("%d", &n);
	for (int k = 1; k <= n; k++)
	{
		for (int i = n - k; i > 0; i--)
		{
			printf(" ");
		}
		if (k != n)
		{
			if (k == 1)
				printf("%c", c);
			else
			{
				printf("%c", c);
				for (int y = 0; y < sum; y++)
				{
					printf(" ");
				}
				printf("%c", c);
				sum += 2;
			}
		}
		else
		{
			for (int j = 0; j < k; j++)
			{
				printf("%c", c);
				if (j != k - 1)
					printf(" ");
			}
		}
		printf("\n");
	}
	return 0;
}