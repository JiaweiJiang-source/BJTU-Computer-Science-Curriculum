#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include<string.h>
#include "leap_year.h"
#include "string_operations.h"
#include "math_operations.h"

// 功能菜单
void printMenu() {
    printf("功能菜单:\n");
    printf("1. 判断是否为闰年\n");
    printf("2. 字符串转换为大写\n");
    printf("3. 计算两个数的最大公约数\n");
    printf("0. 退出\n");
}

int main() {
    int choice;

    while (1) {
        printMenu(); // 打印功能菜单
        printf("请输入功能选项: ");
        scanf("%d", &choice);
        getchar(); // 读取换行符，避免影响后续输入

        switch (choice) {
        case 1: {
            int year;
            printf("请输入年份: ");
            scanf("%d", &year);
            getchar(); // 读取换行符
            if (isLeapYear(year)) {
                printf("%d 是闰年。\n", year);
            }
            else {
                printf("%d 不是闰年。\n", year);
            }
            break;
        }
        case 2: {
            char str[100];
            printf("请输入一个字符串: ");
            fgets(str, sizeof(str), stdin); // 使用 fgets 读取字符串
            str[strcspn(str, "\n")] = '\0'; // 去除 fgets 读取的换行符
            toUpperCase(str);
            printf("转换为大写后的字符串: %s\n", str);
            break;
        }
        case 3: {
            int a, b;
            printf("请输入两个整数: ");
            scanf("%d %d", &a, &b);
            getchar(); // 读取换行符
            int gcd = calculateGCD(a, b);
            printf("最大公约数是: %d\n", gcd);
            break;
        }
        case 0:
            printf("退出程序。\n");
            return 0;
        default:
            printf("无效的选项，请重新输入。\n");
        }
    }

    // 暂停程序，等待用户按任意键关闭窗口
    printf("按任意键关闭此窗口...\n");
    getchar();

    return 0;
}