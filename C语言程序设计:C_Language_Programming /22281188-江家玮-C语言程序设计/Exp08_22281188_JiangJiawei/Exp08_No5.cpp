#include <stdio.h>

void PrintNumbers(int line) {
    if (line < 1) {
        printf("输入参数错误\n");
        return;
    }

    // 打印上半部分（包括中间行）
    for (int i = 1; i <= line; i++) {
        // 打印空格
        for (int j = 0; j < line - i; j++) {
            printf("  ");
        }

        // 打印递增的数字
        for (int j = 1; j <= i; j++) {
            printf("%d ", j);
        }

        // 打印递减的数字
        for (int j = i - 1; j >= 1; j--) {
            printf("%d ", j);
        }

        printf("\n");
    }

    // 打印下半部分（不包括中间行）
    for (int i = line - 1; i >= 1; i--) {
        // 打印空格
        for (int j = 0; j < line - i; j++) {
            printf("  ");
        }

        // 打印递增的数字
        for (int j = 1; j <= i; j++) {
            printf("%d ", j);
        }

        // 打印递减的数字
        for (int j = i - 1; j >= 1; j--) {
            printf("%d ", j);
        }

        printf("\n");
    }
}

int main() {
    int line;
    // 输入行数
    printf("请输入行数（line）：");
    scanf_s("%d", &line);

    if (line < 1)
    {
        printf("输入参数错误\n");
        return 0;
    }
    // 调用函数输出图形
    PrintNumbers(line);

    return 0;
}
