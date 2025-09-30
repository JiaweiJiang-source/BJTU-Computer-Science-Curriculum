#include <stdio.h>

// 定义全局变量，用于存储统计结果
int c1 = 0, c2 = 0, c3 = 0;

// 函数：统计数字 1、2、3 在 n 中的出现次数
void count_digits(int n) {
    // 逐位检查整数 n 的每一位
    while (n > 0) {
        int digit = n % 10;  // 获取最低位数字
        if (digit == 1) {
            c1++;  // 统计数字 1
        }
        else if (digit == 2) {
            c2++;  // 统计数字 2
        }
        else if (digit == 3) {
            c3++;  // 统计数字 3
        }
        n /= 10;  // 去掉最低位数字
    }
}

int main() {
    int n;

    // 输入整数 n
    printf("请输入一个整数：");
    scanf_s("%d", &n);

    // 调用函数统计数字 1、2、3 的出现次数
    count_digits(n);

    // 输出统计结果
    printf("数字1出现的次数：%d\n", c1);
    printf("数字2出现的次数：%d\n", c2);
    printf("数字3出现的次数：%d\n", c3);

    return 0;
}
