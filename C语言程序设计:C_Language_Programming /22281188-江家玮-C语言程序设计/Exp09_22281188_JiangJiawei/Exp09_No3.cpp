#include <stdio.h>

// 递归函数：使用辗转相减法求最大公约数
int gcd(int m, int n) {
    if (m == n) {
        return m;  // 当两个数相等时，返回任意一个数作为最大公约数
    }
    else if (m > n) {
        return gcd(m - n, n);  // 如果 m > n，递归求 gcd(m - n, n)
    }
    else {
        return gcd(m, n - m);  // 如果 m <= n，递归求 gcd(m, n - m)
    }
}

int main() {
    int m, n;

    // 输入两个正整数
    printf("请输入两个正整数 m 和 n (m > 0, n > 0): ");
    scanf_s("%d %d", &m, &n);

    // 调用 gcd 函数计算最大公约数
    int result = gcd(m, n);

    // 输出最大公约数
    printf("最大公约数 gcd(%d, %d) = %d\n", m, n, result);

    return 0;
}
