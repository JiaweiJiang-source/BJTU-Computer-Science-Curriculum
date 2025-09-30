#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main() {
    int a, n;
    int result = 1;

    // 输入 a 和 n
    printf("请输入 a 和 n 的值：");
    scanf("%d %d", &a, &n);

    // 只需要计算 a^n % 1000
    a = a % 1000;  // 防止 a 的初值过大，先对 1000 取模

    // 计算 a 的 n 次方的最后 3 位数 (a^n % 1000)
    for (int i = 0; i < n; i++) {
        result = (result * a) % 1000;  // 每次计算都取模 1000
    }

    // 输出结果
    printf("a^n 的最后 3 位数是：%03d\n", result);

    return 0;
}
