#include <stdio.h>

// 判断一个数是否为完数
int isPerfectNumber(int num) {
    int sum = 1; // 1是所有数的因子

    // 遍历2到num/2，找出所有因子并求和
    for (int i = 2; i <= num / 2; i++) {
        if (num % i == 0) {
            sum += i;
        }
    }

    // 判断是否为完数
    return (sum == num);
}

// 找出m到n之间的所有完数并求和
int sumWanshu(int m, int n) {
    int sum = 0;

    printf("在 %d 到 %d 之间的完数有: ", m, n);
    for (int i = m; i <= n; i++) {
        if (isPerfectNumber(i)) {
            printf("%d ", i);
            sum += i;
        }
    }
    printf("\n");

    return sum;
}

int main() {
    int m = 5, n = 500;

    // 调用函数找出完数并求和
    int result = sumWanshu(m, n);

    // 输出结果
    printf("在 %d 到 %d 之间的所有完数的总和为: %d\n", m, n, result);

    return 0;
}