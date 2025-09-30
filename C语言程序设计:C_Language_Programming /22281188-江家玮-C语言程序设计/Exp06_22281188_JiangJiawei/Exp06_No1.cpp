#include <stdio.h>
#include <ctype.h>  // 用于 tolower() 函数

int main() {
    char str[100];  // 用于存储输入的字符串
    int vowel_count = 0;  // 用于统计元音字母的个数
    printf("请输入一个字符串：");
    fgets(str, sizeof(str), stdin);  // 使用 fgets 读取输入的字符串

    // 遍历字符串中的每个字符
    for (int i = 0; str[i] != '\0'; i++) {
        char ch = tolower(str[i]);  // 将字符转为小写，避免大小写问题
        // 判断是否为元音字母
        if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u') {
            vowel_count++;  // 如果是元音字母，则计数加 1
        }
    }

    // 输出结果
    printf("字符串中元音字母的个数是：%d\n", vowel_count);

    return 0;
}
