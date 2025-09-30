#include <stdio.h>
#include <string.h>

// 从字符串s1中删除所有出现在字符串s2中的字符
void squeeze(char s1[], const char s2[]) {
    int i, j, k;
    int found; // 标记是否找到匹配字符

    // 遍历字符串s1
    for (i = j = 0; s1[i] != '\0'; i++) {
        found = 0; // 初始化标记为未找到

        // 检查当前字符s1[i]是否在s2中
        for (k = 0; s2[k] != '\0'; k++) {
            if (s1[i] == s2[k]) {
                found = 1; // 找到匹配字符
                break;
            }
        }

        // 如果未找到匹配字符，保留该字符
        if (!found) {
            s1[j++] = s1[i];
        }
    }

    // 添加字符串结束符
    s1[j] = '\0';
}

int main() {
    char s1[100]; // 定义字符串s1
    char s2[100]; // 定义字符串s2

    // 输入字符串s1
    printf("请输入字符串s1: ");
    fgets(s1, sizeof(s1), stdin);
    s1[strcspn(s1, "\n")] = '\0'; // 去除fgets读取的换行符

    // 输入字符串s2
    printf("请输入字符串s2: ");
    fgets(s2, sizeof(s2), stdin);
    s2[strcspn(s2, "\n")] = '\0'; // 去除fgets读取的换行符

    // 调用squeeze函数
    squeeze(s1, s2);

    // 输出结果
    printf("处理后的字符串s1: %s\n", s1);

    return 0;
}