#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main() {
    int year;

    // 输入年份
    printf("请输入一个年份: ");
    scanf("%d", &year);

    // 判断是否为闰年
    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
        printf("%d年是闰年。\n", year);
    }
    else {
        printf("%d年不是闰年。\n", year);
    }

    return 0;
}
