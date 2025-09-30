#include <stdio.h>

int main() {
    // 使用 float 类型
    float a_float = 0.123456789;
    float b_float = 0.000000123;
    float c_float = a_float + b_float;

    // 使用 double 类型
    double a_double = 0.123456789;
    double b_double = 0.000000123;
    double c_double = a_double + b_double;

    // 输出结果
    printf("float:\n");
    printf("a + b = %.9f + %.9f = %.9f\n", a_float, b_float, c_float);

    printf("double:\n");
    printf("a + b = %.9f + %.9f = %.9f\n", a_double, b_double, c_double);

    return 0;
}
