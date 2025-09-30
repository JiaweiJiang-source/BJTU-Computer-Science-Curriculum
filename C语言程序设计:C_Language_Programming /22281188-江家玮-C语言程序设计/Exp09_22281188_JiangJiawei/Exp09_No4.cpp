#include <stdio.h>

// 计算 N 阶勒让德多项式的递归函数
double p(int n, double x) {
    // P_0(x) = 1
    if (n == 0) {
        return 1;
    }
    // P_1(x) = x
    else if (n == 1) {
        return x;
    }
    // 递归公式：Pn(x) = ((2n-1) * x * P(n-1)(x) - (n-1) * P(n-2)(x)) / n
    else {
        return ((2 * n - 1) * x * p(n - 1, x) - (n - 1) * p(n - 2, x)) / n;
    }
}

int main() {
    int n;
    double x;

    // 输入 N 和 x
    printf("请输入 N（阶数）和 x 的值: ");
    scanf_s("%d %lf", &n, &x);

    // 计算并输出 N 阶勒让德多项式的值
    double result = p(n, x);
    printf("P_%d(%.2lf) = %.6lf\n", n, x, result);

    return 0;
}
