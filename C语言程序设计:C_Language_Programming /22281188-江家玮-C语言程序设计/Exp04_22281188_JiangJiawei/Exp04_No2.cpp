#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <math.h>  // 用于计算平方根

int main() {
    // 定义系数
    double a, b, c;

    // 输入方程的三个系数
    printf("请输入方程的系数 a, b, c: ");
    scanf("%lf %lf %lf", &a, &b, &c);

    // 判断 a 是否为 0，如果是 0，则不是二次方程
    if (a == 0) {
        printf("这不是一个二次方程。\n");
        return 0;
    }

    // 计算判别式
    double delta = b * b - 4 * a * c;

    // 判断判别式的值
    if (delta > 0) {
        // 两个不同的实数解
        double x1 = (-b + sqrt(delta)) / (2 * a);
        double x2 = (-b - sqrt(delta)) / (2 * a);
        printf("方程的两个实数解为: x1 = %.2lf, x2 = %.2lf\n", x1, x2);
    }
    else if (delta == 0) {
        // 一个实数解（双重根）
        double x = -b / (2 * a);
        printf("方程有一个实数解: x = %.2lf\n", x);
    }
    else {
        // 无实数解，只有复数解
        double realPart = -b / (2 * a);
        double imaginaryPart = sqrt(-delta) / (2 * a);
        printf("方程没有实数解，复数解为: x1 = %.2lf + %.2lfi, x2 = %.2lf - %.2lfi\n",
            realPart, imaginaryPart, realPart, imaginaryPart);
    }

    return 0;
}
