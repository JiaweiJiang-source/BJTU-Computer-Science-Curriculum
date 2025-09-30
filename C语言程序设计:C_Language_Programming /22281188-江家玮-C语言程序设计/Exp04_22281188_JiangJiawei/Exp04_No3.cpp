#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main() {
    char op;
    double num1, num2, result;

    // 输入运算符和两个实数
    printf("请输入运算符（+、-、*、/）以及两个实数（用空格分隔）：");
    scanf("%c %lf %lf", &op, &num1, &num2);

    // 使用 switch 语句处理不同的运算符
    switch (op) {
    case '+':
        result = num1 + num2;
        printf("%.2lf %c %.2lf = %.2lf\n", num1, op, num2, result);
        break;

    case '-':
        result = num1 - num2;
        printf("%.2lf %c %.2lf = %.2lf\n", num1, op, num2, result);
        break;

    case '*':
        result = num1 * num2;
        printf("%.2lf %c %.2lf = %.2lf\n", num1, op, num2, result);
        break;

    case '/':
        if (num2 == 0) {
            printf("除数为零无意义\n");
        }
        else {
            result = num1 / num2;
            printf("%.2lf %c %.2lf = %.2lf\n", num1, op, num2, result);
        }
        break;

    default:
        printf("未知运算符\n");
        break;
    }

    return 0;
}
