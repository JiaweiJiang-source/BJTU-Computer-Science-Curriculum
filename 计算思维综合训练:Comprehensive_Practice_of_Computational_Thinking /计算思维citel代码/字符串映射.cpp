#include<stdio.h>
#include<string.h>
int main() {
	int a[100] = { 0 }, b[100] = { 0 }, i, j, k, p, sum = 0, n = 0;
	int a1[100] = { 0 }, b1[100] = { 0 };
	char s1[102], s2[102];
	scanf("%s %s", s1, s2);
	k = strlen(s1);
	for (i = 0; i < k; i++) {
		p = (int)s1[i];
		a[p]++;
		p = (int)s2[i];
		b[p]++;
	}
	for (i = 'A'; i <= 'Z'; i++) {
		a1[a[i]]++;
		b1[b[i]]++;
	}
	for (i = 0; i < 100; i++) {
		if (a1[i] == b1[i])
			sum++;
	}
	if (sum == 100) {
		printf("YES\n");
		for (i = 'A'; i <= 'Z'; i++) {
			if (a[i] != 0) {
				for (j = 'A'; j <= 'Z'; j++) {
					if (a[i] == b[j]) {
						printf("%c->%c ", i, j);
						b[j] = 0;
						break;
					}
				}
			}
		}
	}
	else printf("NO\n");
	return 0;
}
