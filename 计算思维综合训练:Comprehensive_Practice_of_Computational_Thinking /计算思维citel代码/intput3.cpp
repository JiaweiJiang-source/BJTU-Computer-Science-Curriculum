#include<iostream>
using namespace std;
int main()
{
	long long n, m;
	while (scanf("%lld %lld", &n, &m) != EOF)
	{
		if (n == 0 && m == 0)
		{
			break;
		}
		printf("%lld\n", n + m);
	}
	return 0;
}