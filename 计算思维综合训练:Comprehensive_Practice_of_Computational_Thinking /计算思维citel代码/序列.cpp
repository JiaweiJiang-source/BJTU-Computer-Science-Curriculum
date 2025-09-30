#include <stdio.h>
int main()
{
	long long s[10000];
	s[0] = 0, s[1] = 1;
	int A = 1, B = 1, C = 1;
	long long int number;
	for (int i = 2; i <= 10000; i++)
	{
		long long int a = s[A] * 2;
		long long int b = s[B] * 3;
		long long int c = s[C] * 5;
		number = (c < a&& c < b) ? c : (a < b) ? a : b;
		s[i] = number;
		if (s[i] == a)
			A++;
		if (s[i] == b)
			B++;
		if (s[i] == c)
			C++;
	}
	long long int n;
	while (scanf("%lld", &n) == 1)
	{
		printf("%lld\n", s[n]);
	}
}
