#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <string.h>

// 函数原型
void fun(char* s, char* c, int m, int n) {
    // 检查m是否在有效范围内
    if (m < 1 || m > n) {
        printf("错误：m的值超出范围。\n");
        c[0] = '\0'; // 将目标字符串置为空
        return;
    }

    // 从第m个字符开始复制
    strncpy(c, s + m - 1, n - m + 1);
    c[n - m + 1] = '\0'; // 添加字符串结束符
}

int main() {
    char s[100]; // 源字符串
    char c[100]; // 目标字符串
    int m, n;    // 起始位置和字符串长度

    // 输入源字符串
    printf("请输入源字符串: ");
    fgets(s, sizeof(s), stdin);
    s[strcspn(s, "\n")] = '\0'; // 去除fgets读取的换行符

    // 输入起始位置m
    printf("请输入起始位置m: ");
    scanf("%d", &m);

    // 获取字符串长度
    n = strlen(s);

    // 调用函数进行复制
    fun(s, c, m, n);

    // 输出结果
    printf("从第%d个字符开始的字符串为: %s\n", m, c);

    return 0;
}