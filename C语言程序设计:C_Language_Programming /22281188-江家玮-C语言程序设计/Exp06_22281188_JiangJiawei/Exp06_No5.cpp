#include <stdio.h>

int main() {
    // 定义 k 为比例常数
    float k = 10;  // 选择一个合适的 k 值
    int x = 1.8 * k, y = 1.5 * k, z = 2.0 * k;  // 计算每种硬币的数量

    // 计算总金额，单位为元
    float total_amount = (x / 10.0) + (y / 2.0) + z;

    // 输出每种硬币的数量和总金额
    printf("%d,%d,%d,%.2f\n", x, y, z, total_amount);

    return 0;
}
