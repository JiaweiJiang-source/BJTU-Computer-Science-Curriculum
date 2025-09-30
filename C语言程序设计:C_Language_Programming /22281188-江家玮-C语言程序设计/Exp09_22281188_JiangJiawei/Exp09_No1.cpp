#include <stdio.h>
#include <math.h>

// 定义方程 f(x) 和其导数 f'(x)
double f(double x, double a, double b, double c, double d) {
    return a * x * x * x + b * x * x + c * x + d;
}

double f_prime(double x, double a, double b, double c) {
    return 3 * a * x * x + 2 * b * x + c;
}

int main() {
    double a, b, c, d, x0, x1, tolerance;
    int max_iterations = 1000;

    // 输入方程的系数
    printf("请输入方程的系数 a, b, c, d (空格分隔): ");
    scanf_s("%lf %lf %lf %lf", &a, &b, &c, &d);

    // 设置初始值和容忍度
    x0 = 1.0;  // 初始猜测值
    tolerance = 1e-6;  // 精度要求

    // 迭代求解
    int iterations = 0;
    do {
        // 计算 f(x0) 和 f'(x0)
        double fx0 = f(x0, a, b, c, d);
        double fx0_prime = f_prime(x0, a, b, c);

        // 防止除以零的情况
        if (fabs(fx0_prime) < 1e-12) {
            printf("导数值接近零，无法继续迭代\n");
            return -1;
        }

        // 计算下一次迭代值
        x1 = x0 - fx0 / fx0_prime;

        // 判断是否收敛
        if (fabs(x1 - x0) < tolerance) {
            break;
        }

        // 更新x0进行下一次迭代
        x0 = x1;
        iterations++;
    } while (iterations < max_iterations);

    // 输出结果
    printf("方程的近似解为：%.6lf\n", x1);
    printf("迭代次数：%d\n", iterations);

    return 0;
}
