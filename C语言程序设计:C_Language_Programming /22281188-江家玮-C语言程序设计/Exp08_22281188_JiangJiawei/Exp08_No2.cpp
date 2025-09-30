#include <stdio.h>
#include <math.h>

#define PI 3.1415926

// 计算四边形的面积
double CalculateArea(double a, double b, double c, double d, double alpha) {
    // 计算半周长 s
    double s = (a + b + c + d) / 2.0;

    // 将角度转换为弧度
    double angle_rad = alpha * (PI / 180.0);

    // 计算余弦值
    double result = cos(angle_rad);

    // 计算面积
    double area = sqrt((s - a) * (s - b) * (s - c) * (s - d) -
        a * b * c * d * pow(result, 2));

    return area;
}

int main() {
    // 输入四边形的四条边长和对角线之和
    double a = 3, b = 4, c = 5, d = 5;
    double alpha = 145.0; // 对角线之和为145度，这里假设是给定的角度

    // 计算面积
    double area = CalculateArea(a, b, c, d, alpha);

    // 输出结果
    printf("四边形的面积为：%.2f\n", area);

    return 0;
}
