#include <stdio.h>

// 数据块交换函数
void BlockExchange(int nArray[], int n, int m) {
    int temp[100]={0};  // 用于存储前 m 个数

    // 将前 m 个数存储到 temp 数组中
    for (int i = 0; i < m; i++) {
        temp[i] = nArray[i];
    }

    // 将后 m 个数移到前 m 个数的位置
    for (int i = 0; i < m; i++) {
        nArray[i] = nArray[n - m + i];
    }

    // 将之前存储在 temp 数组中的前 m 个数移到后 m 个数的位置
    for (int i = 0; i < m; i++) {
        nArray[n - m + i] = temp[i];
    }
}

// 主函数
int main() {
    int n = 20;  // 预设数据个数
    int m;
    int nArray[20] = { 23, 19, 88, 71, 23, 55, 32, 18, 9, 2, 22, 72, 66, 55, 80, 33, 17, 0, 102, 7 };

    // 输入 m 的值
    printf("请输入 m 的值（m < n/2, 例如 n = 20）: ");
    scanf_s("%d", &m);

    // 检查 m 是否满足条件 m < n/2
    if (m >= n / 2) {
        printf("输入错误，m 必须小于 n/2\n");
        return 1;
    }

    // 输出原始数组
    printf("原始数组: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", nArray[i]);
    }
    printf("\n");

    // 调用 BlockExchange 函数进行数据块交换
    BlockExchange(nArray, n, m);

    // 输出交换后的数组
    printf("交换后的数组: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", nArray[i]);
    }
    printf("\n");

    return 0;
}
