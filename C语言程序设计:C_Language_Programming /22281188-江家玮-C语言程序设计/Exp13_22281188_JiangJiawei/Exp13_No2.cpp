#include <stdio.h>
#include <string.h>
#include <ctype.h>

// 检查两个字符串是否由同样的字符组成
int areSameCharacters(char* str1, char* str2) {
    int count1[256] = { 0 }; // 用于统计str1中每个字符的出现次数
    int count2[256] = { 0 }; // 用于统计str2中每个字符的出现次数

    // 统计str1中每个字符的出现次数
    while (*str1) {
        count1[tolower(*str1)]++; // 忽略大小写
        str1++;
    }

    // 统计str2中每个字符的出现次数
    while (*str2) {
        count2[tolower(*str2)]++; // 忽略大小写
        str2++;
    }

    // 比较两个字符串的字符统计结果
    for (int i = 0; i < 256; i++) {
        if (count1[i] != count2[i]) {
            return 0; // 字符统计结果不同，返回0
        }
    }

    return 1; // 字符统计结果相同，返回1
}

// 判断一个字符串是否可以通过另一个字符串重排得到
int canRearrange(char* str1, char* str2) {
    return areSameCharacters(str1, str2); // 直接调用areSameCharacters函数
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

    // 测试areSameCharacters函数
    if (areSameCharacters(str1, str2)) {
        printf("两个字符串由同样的字符组成。\n");
    }
    else {
        printf("两个字符串不包含同样的字符。\n");
    }

    // 测试canRearrange函数
    if (canRearrange(str1, str2)) {
        printf("第二个字符串可以通过第一个字符串重排得到。\n");
    }
    else {
        printf("第二个字符串不能通过第一个字符串重排得到。\n");
    }

    return 0;
}