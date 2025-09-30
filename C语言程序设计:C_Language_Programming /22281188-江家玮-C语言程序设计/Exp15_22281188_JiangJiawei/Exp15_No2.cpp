#include <stdio.h>
#include <string.h>

// 统计子字符串在字符串中出现的次数
int proc(char* str, char* sub) {
    int count = 0;
    int subLen = strlen(sub);

    // 遍历字符串，查找子字符串
    for (int i = 0; str[i] != '\0'; i++) {
        // 检查从当前位置开始的子字符串是否匹配
        if (strncmp(&str[i], sub, subLen) == 0) {
            count++;
            i += subLen - 1; // 跳过已匹配的子字符串
        }
    }

    return count;
}

int main() {
    char str[100], sub[100];

    // 输入字符串
    printf("请输入字符串: ");
    fgets(str, sizeof(str), stdin);
    str[strcspn(str, "\n")] = '\0'; // 去除fgets读取的换行符

    // 输入子字符串
    printf("请输入子字符串: ");
    fgets(sub, sizeof(sub), stdin);
    sub[strcspn(sub, "\n")] = '\0'; // 去除fgets读取的换行符

    // 调用函数统计子字符串出现的次数
    int count = proc(str, sub);

    // 输出结果
    printf("子字符串 \"%s\" 在字符串 \"%s\" 中出现的次数为: %d\n", sub, str, count);

    return 0;
}