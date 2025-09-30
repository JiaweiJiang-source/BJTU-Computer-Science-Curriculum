#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// 判断是否为中文字符（UTF-8编码）
bool isChineseChar(const char* str, int index) {
    // UTF-8中文字符的前两个字节满足特定条件
    return (str[index] & 0x80) && (str[index + 1] & 0x80);
}

// 判断字符串是否为回文
bool isPalindrome(const char* str) {
    int len = strlen(str);
    int i = 0, j = len - 1;

    while (i < j) {
        // 如果是中文字符，跳过两个字节
        if (isChineseChar(str, i)) {
            if (str[i] != str[j - 1] || str[i + 1] != str[j]) {
                return false;
            }
            i += 2;
            j -= 2;
        }
        else {
            // 如果是单字节字符，直接比较
            if (str[i] != str[j]) {
                return false;
            }
            i++;
            j--;
        }
    }

    return true;
}

int main() {
    char str[100];

    // 输入字符串
    printf("请输入一个字符串: ");
    fgets(str, sizeof(str), stdin);
    str[strcspn(str, "\n")] = '\0'; // 去除fgets读取的换行符

    // 判断是否为回文
    if (isPalindrome(str)) {
        printf("字符串 \"%s\" 是回文。\n", str);
    }
    else {
        printf("字符串 \"%s\" 不是回文。\n", str);
    }

    return 0;
}