#include<iostream>
#include<algorithm>
using namespace std;
int a[10010];
int n;
long long find() {
	sort(a, a + n);
	return a[n - 1] - a[0];
}
int main() {
	scanf("%d", &n);
	for (int i = 0; i < n; i++)
		scanf("%d", &a[i]);
	printf("%lld\n", find());
	return 0;
}