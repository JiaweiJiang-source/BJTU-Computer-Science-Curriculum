#include<iostream>
#include<algorithm>
using namespace std;

int main() {
	int a[10010];
	int n;
	int minn = 100000000000;
	scanf("%d", &n);
	for (int i = 0; i < n; i++)
		scanf("%d", &a[i]);
	sort(a, a + n);
	for (int i = 0; i < n - 1; i++) {
		minn = min(minn, a[i + 1] - a[i]);
	}
	return minn;
	printf("%d\n", minn);
	return 0;
}