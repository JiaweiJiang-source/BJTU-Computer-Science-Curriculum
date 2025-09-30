#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// 获取字符串中最长的单词
void getLongestWord(const char* str, char* longestWord) {
    int maxLength = 0;
    int currentLength = 0;
    int start = 0;
    int i;

    for (i = 0; str[i] != '\0'; i++) {
        // 如果是字母，增加当前单词长度
        if (isalpha(str[i])) {
            currentLength++;
        }
        else {
            // 如果不是字母，比较当前单词长度和最大长度
            if (currentLength > maxLength) {
                maxLength = currentLength;
                strncpy(longestWord, &str[start], maxLength);
                longestWord[maxLength] = '\0'; // 添加字符串结束符
            }
            // 重置当前单词长度和起始位置
            currentLength = 0;
            start = i + 1;
        }
    }

    // 检查最后一个单词
    if (currentLength > maxLength) {
        maxLength = currentLength;
        strncpy(longestWord, &str[start], maxLength);
        longestWord[maxLength] = '\0'; // 添加字符串结束符
    }
}

int main() {
    char line[1000]; // 存储每行输入
    char longestWord[1000] = ""; // 存储最长的单词
    char currentWord[1000] = ""; // 存储当前行的最长单词
    int maxLength = 0;

    printf("请输入多行字符（按Ctrl+Z结束输入）:\n");

    // 读取多行输入
    while (fgets(line, sizeof(line), stdin) != NULL) {
        // 去除换行符
        line[strcspn(line, "\n")] = '\0';

        // 获取当前行的最长单词
        getLongestWord(line, currentWord);

        // 比较当前行的最长单词和全局最长单词
        if (strlen(currentWord) > maxLength) {
            maxLength = strlen(currentWord);
            strcpy(longestWord, currentWord);
        }
    }

    // 输出结果
    if (maxLength > 0) {
        printf("最长的单词是: %s\n", longestWord);
    }
    else {
        printf("没有输入有效的单词。\n");
    }

    return 0;
}