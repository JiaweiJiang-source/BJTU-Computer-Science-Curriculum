#include <stdio.h>
#include <math.h>

// 计算多项式的函数
double PolyFunc(double dArray[], int item, double x) {
    double result = 0.0;

    // 计算多项式的值
    for (int i = 0; i <= item; i++) {
        result += dArray[i] * pow(x, i);  // dArray[i] * x^i
    }

    return result;  // 返回多项式值
}

int main() {
    int n;

    // 输入多项式的阶数 n
    printf("请输入多项式的阶数 n: ");
    scanf_s("%d", &n);

    // 输入多项式的系数 a0, a1, ..., an
    double dArray[100];  // 存储多项式系数的数组
    printf("请输入多项式的系数 a0, a1, ..., a%d:\n", n);
    for (int i = 0; i <= n; i++) {
        printf("a%d = ", i);
        scanf_s("%lf", &dArray[i]);
    }

    // 输入 x 值
    double x;
    printf("请输入 x 的值: ");
    scanf_s("%lf", &x);

    // 计算多项式的值
    double result = PolyFunc(dArray, n, x);

    // 输出计算结果
    printf("多项式的值为: %.6lf\n", result);

    return 0;
}
