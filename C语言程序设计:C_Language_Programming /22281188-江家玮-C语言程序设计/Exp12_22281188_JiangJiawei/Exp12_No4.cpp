#include <stdio.h>

// 使用数组实现
void select1(double a[], double b[], int n, double x) {
    int index = 0; // 用于记录数组a的当前索引

    // 遍历数组b，将大于等于x的数复制到数组a中
    for (int i = 0; i < n; i++) {
        if (b[i] >= x) {
            a[index++] = b[i];
        }
    }

    // 将数组a的剩余部分填充为0（可选）
    for (int i = index; i < n; i++) {
        a[i] = 0;
    }
}

// 使用指针实现
void select2(double* a, double* b, int n, double x) {
    double* pA = a; // 指向数组a的指针
    double* pB = b; // 指向数组b的指针

    // 遍历数组b，将大于等于x的数复制到数组a中
    for (int i = 0; i < n; i++) {
        if (*pB >= x) {
            *pA = *pB;
            pA++;
        }
        pB++;
    }

    // 将数组a的剩余部分填充为0（可选）
    while (pA < a + n) {
        *pA = 0;
        pA++;
    }
}

int main() {
    double b[10] = { 1.5, 3.2, 4.8, 2.1, 5.0, 3.7, 6.4, 2.9, 4.5, 1.0 };
    double a1[10] = { 0 }; // 用于存储数组实现的结果
    double a2[10] = { 0 }; // 用于存储指针实现的结果
    int n = 10;
    double x = 3.5;

    // 使用数组实现
    select1(a1, b, n, x);
    printf("使用数组实现的结果:\n");
    for (int i = 0; i < n; i++) {
        printf("%.2f ", a1[i]);
    }
    printf("\n");

    // 使用指针实现
    select2(a2, b, n, x);
    printf("使用指针实现的结果:\n");
    for (int i = 0; i < n; i++) {
        printf("%.2f ", a2[i]);
    }
    printf("\n");

    return 0;
}