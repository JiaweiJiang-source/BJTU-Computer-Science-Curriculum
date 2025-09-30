#include <stdio.h>
#include <math.h>  // 引入math.h库以使用PI常数和数学函数
#define M_PI 3.1415926
// 计算圆球体积的函数
double volume(double radius) {
    return (4.0 / 3.0) * M_PI * pow(radius, 3);  // 使用M_PI来获取π的值
}

// 计算圆球表面积的函数
double SurfaceArea(double radius) {
    return 4 * M_PI * pow(radius, 2);  // 使用M_PI来获取π的值
}

int main() {
    double radius;

    // 输入圆球的半径
    printf("请输入圆球的半径：");
    scanf_s("%lf", &radius);

    // 计算圆球的体积和表面积
    double vol = volume(radius);
    double area = SurfaceArea(radius);

    // 输出圆球的体积和表面积
    printf("圆球的体积为：%.2lf\n", vol);
    printf("圆球的表面积为：%.2lf\n", area);

    return 0;
}
