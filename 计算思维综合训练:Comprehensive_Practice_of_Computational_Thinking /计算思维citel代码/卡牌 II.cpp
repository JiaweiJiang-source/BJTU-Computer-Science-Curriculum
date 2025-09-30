#include <bits/stdc++.h>
using namespace std;
#define maxn 100005
int n, Q;
int a[maxn], q, tt = 1;
int pre[maxn];
int main() {
    while (cin >> n >> Q) {
        printf("Case #%d:\n", tt++);
        for (int i = 0; i < n; i++) {
            scanf("%d", &a[i]);
            pre[a[i]] = i;
        }
        sort(a, a + n);
        for (int i = 0; i < Q; i++) {
            cin >> q;
            int* it = lower_bound(a, a + n, q);
            if (it != a + n && *it == q)
                printf("%d from %d to %d\n", q, pre[q] + 1, it - a + 1);
            else
                printf("%d not found\n", q);
        }
    }
    return 0;
}