#include <stdio.h>
int main(void)
{
	int local[2] = { 0 }, i = 0, j = 0, n;
	long long min, temp, arr[10005];
	scanf("%d", &n);
	for (int k = 0; k < n; k++) {
		scanf("%lld", &arr[k]);
	}
	for (min = 9223372036854775807, i = 0; i < n - 1; i++)
	{
		for (j = i + 1; j < n; j++)
		{
			temp = (arr[j] > arr[i]) ? arr[j] - arr[i] : arr[i] - arr[j];
			if (temp < min) {
				min = temp;
				local[0] = i + 1;
				local[1] = j + 1;
			}
		}
	}
	printf("%lld %d %d\n", min, local[0], local[1]);
}