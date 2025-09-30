#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main() {
    int n;
    double sum = 0.0;

    // 输入 n
    printf("请输入 n 的值：");
    scanf("%d", &n);

    // 计算 SUM 的值
    for (int i = 1; i <= n; i++) {
        sum += 1.0 / i;  // 累加每个倒数
    }

    // 输出结果，保留 6 位小数
    printf("SUM = %.6lf\n", sum);

    return 0;
}

