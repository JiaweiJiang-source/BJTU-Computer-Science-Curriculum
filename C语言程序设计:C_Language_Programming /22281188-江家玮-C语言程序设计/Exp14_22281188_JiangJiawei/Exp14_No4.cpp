#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 字符串拼接函数
char* StrCat(char* Str1, char* Str2) {
    // 计算拼接后的字符串长度
    int len1 = strlen(Str1);
    int len2 = strlen(Str2);
    int totalLen = len1 + len2 + 3; // 加1个空格和1个'!'，再加1个字符串结束符

    // 分配足够的内存来存储拼接后的字符串
    char* result = (char*)malloc(totalLen * sizeof(char));
    if (result == NULL) {
        printf("内存分配失败！\n");
        return NULL;
    }

    // 复制Str1到结果字符串
    strcpy(result, Str1);

    // 在Str1后面添加一个空格
    strcat(result, " ");

    // 将Str2拼接到结果字符串
    strcat(result, Str2);

    // 在结果字符串末尾添加'!'
    strcat(result, "!");

    return result;
}

int main() {
    char str1[100], str2[100];

    // 输入第一个字符串
    printf("请输入第一个字符串: ");
    fgets(str1, sizeof(str1), stdin);
    str1[strcspn(str1, "\n")] = '\0'; // 去除fgets读取的换行符

    // 输入第二个字符串
    printf("请输入第二个字符串: ");
    fgets(str2, sizeof(str2), stdin);
    str2[strcspn(str2, "\n")] = '\0'; // 去除fgets读取的换行符

    // 调用StrCat函数进行拼接
    char* result = StrCat(str1, str2);

    // 输出结果
    if (result != NULL) {
        printf("拼接后的字符串为: %s\n", result);
        free(result); // 释放动态分配的内存
    }

    return 0;
}