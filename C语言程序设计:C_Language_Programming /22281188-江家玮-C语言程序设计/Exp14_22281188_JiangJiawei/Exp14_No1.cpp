#include <stdio.h>
#include <math.h>

// 计算阶乘
double factorial(int n) {
    double result = 1.0;
    for (int i = 1; i <= n; i++) {
        result *= i;
    }
    return result;
}

// 计算公式的值
double sumFunc(double x) {
    double sum = 0.0;
    double term = x;
    int n = 1;

    while (fabs(term) >= 1e-5) {
        sum += term;
        n++;
        term = pow(-1, n - 1) * pow(x, n) / factorial(n);
    }

    return sum;
}

int main() {
    double x;

    // 输入实数x
    printf("请输入一个实数x: ");
    scanf_s("%lf", &x);

    // 调用函数计算并输出结果
    double result = sumFunc(x);
    printf("计算结果为: %.2f\n", result);

    return 0;
}