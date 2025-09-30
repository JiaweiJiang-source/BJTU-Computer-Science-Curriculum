#include <stdio.h>
#include <ctype.h>
#include <string.h>

// 将字符串中每一个单词的第一个字母转换为大写
void convert(char* p) {
    int len = strlen(p);
    int i = 0;

    // 处理第一个单词的第一个字母
    if (len > 0) {
        p[0] = toupper(p[0]);
    }

    // 遍历字符串，将每个单词的第一个字母转换为大写
    for (i = 1; i < len; i++) {
        if (p[i - 1] == ' ' && p[i] != ' ') {
            p[i] = toupper(p[i]);
        }
    }
}

int main() {
    char str[100];

    // 输入字符串
    printf("请输入一个由小写字母组成的字符串: ");
    fgets(str, sizeof(str), stdin);
    str[strcspn(str, "\n")] = '\0'; // 去除fgets读取的换行符

    // 调用函数进行转换
    convert(str);

    // 输出转换后的字符串
    printf("转换后的字符串为: %s\n", str);

    return 0;
}