#include <stdio.h>

// 函数：计算 1 到 limit 内所有正整数的立方和
int SumCube(int limit) {
    if (limit <= 0) {
        return 0;  // 如果 limit 非正整数，返回 0
    }

    int sum = 0;
    // 计算立方和
    for (int i = 1; i <= limit; i++) {
        sum += i * i * i;  // 立方：i^3
    }

    return sum;  // 返回立方和
}

int main() {
    int limit;

    // 输入 limit
    printf("请输入一个正整数 limit：");
    scanf_s("%d", &limit);

    // 调用 SumCube 函数计算结果
    int result = SumCube(limit);

    // 输出结果
    if (result == 0) {
        printf("输入无效，limit 必须为正整数。\n");
    }
    else {
        printf("1 到 %d 的正整数立方和为：%d\n", limit, result);
    }

    return 0;
}
