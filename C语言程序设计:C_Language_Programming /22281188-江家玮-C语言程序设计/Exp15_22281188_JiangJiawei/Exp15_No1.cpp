#include <stdio.h>

// 打印图案的函数
void picture(int line) {
    // 打印上半部分
    for (int i = 1; i <= line; i++) {
        // 打印前面的空格
        for (int j = 1; j <= line - i; j++) {
            printf(" ");
        }

        // 打印左边的 '@'
        printf("@");

        // 打印中间的 '#'
        for (int j = 1; j <= 2 * i - 3; j++) {
            printf("#");
        }

        // 打印右边的 '@'
        if (i > 1) {
            printf("@");
        }

        // 换行
        printf("\n");
    }

    // 打印下半部分
    for (int i = line - 1; i >= 1; i--) {
        // 打印前面的空格
        for (int j = 1; j <= line - i; j++) {
            printf(" ");
        }

        // 打印左边的 '@'
        printf("@");

        // 打印中间的 '#'
        for (int j = 1; j <= 2 * i - 3; j++) {
            printf("#");
        }

        // 打印右边的 '@'
        if (i > 1) {
            printf("@");
        }

        // 换行
        printf("\n");
    }
}

int main() {
    int line;

    // 输入图案的最长行所在的行数
    printf("请输入图案的最长行所在的行数: ");
    scanf_s("%d", &line);

    // 调用函数打印图案
    picture(line);

    return 0;
}