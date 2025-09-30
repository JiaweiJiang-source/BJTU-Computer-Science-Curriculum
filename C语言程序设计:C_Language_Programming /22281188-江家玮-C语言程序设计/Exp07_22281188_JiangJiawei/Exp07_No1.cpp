#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

// 设计 min 函数，返回三个数中的最小值
double min(double x, double y, double z) {
    double smallest = x;  // 假设 x 是最小的
    if (y < smallest) {
        smallest = y;  // 如果 y 更小，更新 smallest
    }
    if (z < smallest) {
        smallest = z;  // 如果 z 更小，更新 smallest
    }
    return smallest;
}

int main() {
    double a, b, c;

    // 输入三个实数
    printf("请输入三个实数：");
    scanf("%lf %lf %lf", &a, &b, &c);

    // 调用 min 函数得到最小值
    double result = min(a, b, c);

    // 输出最小值
    printf("三个数中的最小值是：%.2f\n", result);

    return 0;
}
