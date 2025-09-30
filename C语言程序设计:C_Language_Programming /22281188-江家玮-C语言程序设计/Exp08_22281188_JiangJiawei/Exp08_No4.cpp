#include <stdio.h>

// 函数声明
int sequence(int n);

int main() {
    int n;

    // 输入n的值
    printf("请输入正整数n：");
    scanf_s("%d", &n);

    // 输入有效性检查
    if (n < 1) {
        printf("请输入一个大于等于1的正整数。\n");
        return 1;  // 程序结束
    }

    // 输出数列的第n项
    printf("数列中第 %d 项的值为：%d\n", n, sequence(n));

    return 0;
}

// 计算数列第n项的函数
int sequence(int n) {
    if (n == 1) {
        return 0;  // 第1项
    }
    else if (n == 2) {
        return 1;  // 第2项
    }
    else if (n == 3) {
        return 2;  // 第3项
    }

    // 定义数组存储数列的每一项
    int a[100];  // 假设n的最大值为100
    a[0] = 0;
    a[1] = 1;
    a[2] = 2;
    a[3] = 3;

    // 通过递推公式计算数列的第n项
    for (int i = 4; i <= n; i++) {
        a[i] = a[i - 1] + a[i - 2] + a[i - 3];
    }

    return a[n - 1];  // 返回第n项的值
}
