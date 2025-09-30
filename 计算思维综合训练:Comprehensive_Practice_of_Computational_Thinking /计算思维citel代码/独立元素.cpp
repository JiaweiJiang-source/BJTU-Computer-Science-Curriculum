#include<cstdio>
#include<cstdlib>
#include<algorithm>
using namespace std;
const int N = 11000;
int A[N];
int main()
{
	int n;
	scanf("%d", &n);
	for (int i = 1; i <= n; i++)
		scanf("%d", &A[i]);
	sort(A + 1, A + 1 + n);
	printf("%d", A[n]);
	for (int i = n - 1; i >= 1; i--)
	{
		if (A[i] == A[i + 1]) continue;
		printf(" %d", A[i]);
	}
	printf("\n");
	return 0;
}
