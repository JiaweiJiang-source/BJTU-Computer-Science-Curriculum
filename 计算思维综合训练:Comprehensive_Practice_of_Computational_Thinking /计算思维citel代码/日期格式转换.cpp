#include<stdio.h>
int main() {
    int y, m, d;
    while (scanf("%d-%d-%d", &y, &m, &d) == 3) {
        printf("%02d/%02d/%04d\n", m, d, y);
    }
    return 0;
}