#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>

int main() {
    int a, b, c;

    // 输入三条边的长度
    printf("请输入三角形的三条边的长度：");
    scanf("%d %d %d", &a, &b, &c);

    // 判断是否为三角形
    if (a <= 0 || b <= 0 || c <= 0 || a + b <= c || a + c <= b || b + c <= a) {
        printf("non-triangle\n");
    }
    else if (a == b && b == c) {
        printf("equilateral triangle\n");  // 等边三角形
    }
    else if (a == b || b == c || a == c) {
        printf("isosceles triangle\n");  // 等腰三角形
    }
    else {
        printf("triangle\n");  // 一般三角形
    }

    return 0;
}
