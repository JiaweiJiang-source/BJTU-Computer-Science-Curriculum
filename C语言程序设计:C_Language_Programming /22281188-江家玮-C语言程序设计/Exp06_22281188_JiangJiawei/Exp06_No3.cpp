#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

// 函数：计算最大公约数（GCD）
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// 函数：简化分数
void simplify(int* numerator, int* denominator) {
    int gcd_value = gcd(*numerator, *denominator);
    *numerator /= gcd_value;
    *denominator /= gcd_value;
}

int main() {
    int numerator1, denominator1;  // 分数1的分子和分母
    int numerator2, denominator2;  // 分数2的分子和分母
    char op;  // 操作符

    // 输入分数1，操作符，分数2
    printf("请输入第一个分数 (格式：a/b)：");
    scanf("%d/%d", &numerator1, &denominator1);
    printf("请输入操作符 (+, -, *, /)：");
    scanf(" %c", &op);  // 注意在%c前有空格，避免读取换行符
    printf("请输入第二个分数 (格式：a/b)：");
    scanf("%d/%d", &numerator2, &denominator2);

    int result_numerator = 0;  // 结果的分子
    int result_denominator = 0;  // 结果的分母

    switch (op) {
    case '+':  // 分数加法
        result_numerator = numerator1 * denominator2 + numerator2 * denominator1;
        result_denominator = denominator1 * denominator2;
        break;
    case '-':  // 分数减法
        result_numerator = numerator1 * denominator2 - numerator2 * denominator1;
        result_denominator = denominator1 * denominator2;
        break;
    case '*':  // 分数乘法
        result_numerator = numerator1 * numerator2;
        result_denominator = denominator1 * denominator2;
        break;
    case '/':  // 分数除法
        result_numerator = numerator1 * denominator2;
        result_denominator = denominator1 * numerator2;
        break;
    default:
        printf("无效的操作符！\n");
        return 1;
    }

    // 简化分数
    simplify(&result_numerator, &result_denominator);

    // 输出结果
    if (result_denominator < 0) {  // 如果分母是负数，将其移到分子前
        result_numerator = -result_numerator;
        result_denominator = -result_denominator;
    }

    printf("结果是：%d/%d\n", result_numerator, result_denominator);

    return 0;
}
