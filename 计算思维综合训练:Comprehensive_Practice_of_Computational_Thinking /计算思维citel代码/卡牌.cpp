#include <iostream>
#include <algorithm>
using namespace std;
const int N = 10010;
int n, m, b;
int a[N];
int cnt = 1;
int main()
{
    while (~scanf("%d%d", &n, &m))
    {
        for (int i = 0; i < n; i++) scanf("%d", &a[i]);
        sort(a, a + n);
        printf("Case #%d:\n", cnt++);
        for (int i = 0; i < m; i++)
        {
            scanf("%d", &b);
            int t = lower_bound(a, a + n, b) - a;
            int dif = upper_bound(a, a + n, b) - lower_bound(a, a + n, b);
            if (!dif) printf("%d not found\n", b);
            else printf("%d found at %d\n", b, t + 1);
        }
    }
    return 0;
}
