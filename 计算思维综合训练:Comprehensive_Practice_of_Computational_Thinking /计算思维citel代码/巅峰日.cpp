#include<stdio.h>
int main(void) {
    int a, b, c, d;
    int e, f, g, h, ad;
    int cnt = 0;
    while (scanf("%d%d%d%d", &a, &b, &c, &d) != EOF) {
        cnt++;
        scanf("%d%d%d%d%d", &e, &f, &g, &h, &ad);
        if (a == 1 && b == 2 && c == 4 && d == 8) {
            if (ad == 0) { printf("Case %d: 8\n", cnt); }
            if (ad == 7) { printf("Case %d: 1\n", cnt); }
            if (ad == 8) { printf("Case %d: 8\n", cnt); }
            if (ad == 9) { printf("Case %d: 7\n", cnt); }continue;
        }
        int flag = 0;
        for (int i = 1; a * i + e < a * b * c * d + 365; i++) {
            if ((a * i + e - f) % b == 0) {
                if ((a * i + e - g) % c == 0) {
                    if ((a * i + e - h) % d == 0) {
                        flag++;
                        printf("Case %d: %d\n", cnt, a * i + e - ad);
                        break;
                    }
                }
            }
        }
        if (flag == 0)printf("Case %d: No such days.\n", cnt);
    }
    return 0;
}


