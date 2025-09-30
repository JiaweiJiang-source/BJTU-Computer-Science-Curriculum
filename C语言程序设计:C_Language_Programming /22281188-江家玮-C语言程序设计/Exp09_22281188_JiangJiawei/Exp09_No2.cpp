#include <stdio.h>

// 计算阶乘的函数
long long factorial(int n) {
    long long result = 1;
    for (int i = 1; i <= n; i++) {
        result *= i;  // 计算阶乘
    }
    return result;
}

int main() {
    int n;
    long long sum = 0;

    // 输入n
    printf("请输入n的值: ");
    scanf_s("%d", &n);

    // 计算阶乘和
    for (int i = 1; i <= n; i++) {
        sum += factorial(i);  // 累加阶乘
    }

    // 输出结果
    printf("y = 1! + 2! + 3! + ... + %d! = %lld\n", n, sum);

    return 0;
}
