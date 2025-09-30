#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <math.h>

// 判断一个数是否是素数
int is_prime(int num) {
    if (num <= 1) return 0;  // 小于等于1的数不是素数
    if (num == 2) return 1;  // 2是素数
    if (num % 2 == 0) return 0;  // 排除偶数
    int limit = sqrt(num);  // 只需要检查到 sqrt(num) 处
    for (int i = 3; i <= limit; i += 2) {  // 从3开始，检查所有奇数
        if (num % i == 0) {
            return 0;  // 能被整除，说明不是素数
        }
    }
    return 1;  // 是素数
}

int main() {
    int lower, upper;

    // 输入范围
    printf("请输入范围（格式：下限 上限）：");
    scanf("%d %d", &lower, &upper);

    // 从上限开始向下查找最大素数
    for (int i = upper; i >= lower; i--) {
        if (is_prime(i)) {
            printf("该范围内的最大素数是：%d\n", i);
            return 0;  // 找到最大素数后退出
        }
    }

    printf("该范围内没有素数。\n");
    return 0;
}
