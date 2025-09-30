#include <stdio.h>

int main() {
    int n;
    double S = 0.0;

    // 输入n
    printf("请输入一个整数 n：");
    scanf_s("%d", &n);

    // 计算多项式 S
    for (int k = 1; k <= n; k++) {
        // 计算分母 1 + 2 + ... + k, 即 k*(k+1)/2
        double denominator = (k * (k + 1)) / 2.0;
        // 累加到 S 中
        S += 1.0 / denominator;
    }

    // 输出结果，保留6位小数
    printf("S = %.6f\n", S);

    return 0;
}
