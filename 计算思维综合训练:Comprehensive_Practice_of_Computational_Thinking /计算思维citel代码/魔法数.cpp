#include<stdio.h>
#include<math.h>
int main()
{
	int x, N, c = 0;
	while (scanf("%d %d", &x, &N) == 2)
	{
		int arr[105]; int flag = 0;
		for (int i = 1; i <= N; i++)
			arr[i] = pow(i, x);
		printf("Case %d:\n", ++c);
		for (int i = 1; i <= N; i++)
		{
			for (int j = i; j <= N; j++)
			{
				for (int k = j; k <= N; k++)
				{
					int sum = arr[i] + arr[j] + arr[k];
					for (int t = k + 1; t <= N; t++)
					{
						if (sum == arr[t])
						{
							flag = 1;
							printf("%d^%d+%d^%d+%d^%d=%d^%d\n", i, x, j, x, k, x, t, x);
							break;
						}
						if (arr[t] > sum)break;
					}
				}
			}
		}
		if (!flag)
			printf("No such numbers.\n");
	}
	return 0;
}

