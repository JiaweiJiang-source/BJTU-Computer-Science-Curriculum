#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>

int main() {
    int h, m;

    // 输入时间
    while (scanf("%d %d", &h, &m) != EOF) {
        // 计算时针和分针的位置
        double hour_angle = 30 * h + 0.5 * m;  // 时针位置
        double minute_angle = 6 * m;            // 分针位置

        // 计算两者的夹角
        double angle = fabs(hour_angle - minute_angle); // 绝对值差

        // 计算最小夹角
        if (angle > 180) {
            angle = 360 - angle;  // 取最小夹角
        }

        // 输出结果
        printf("%d:%02d时针和分针的夹角为%.1f度\n", h, m, angle);
    }

    return 0;
}
