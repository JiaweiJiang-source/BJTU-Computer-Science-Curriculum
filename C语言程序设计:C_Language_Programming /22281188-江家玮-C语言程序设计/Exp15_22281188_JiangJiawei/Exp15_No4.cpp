#include <stdio.h>
#include <ctype.h>
#include<string>
#include<string.h>

// 将字符串中的所有数字字符转换成整数
int ConvertInt(char* str) {
    int result = 0;

    // 遍历字符串，将数字字符转换为整数
    for (int i = 0; str[i] != '\0'; i++) {
        if (isdigit(str[i])) { // 检查字符是否为数字
            result = result * 10 + (str[i] - '0'); // 将字符转换为整数并累加
        }
    }

    return result;
}

int main() {
    char str[100];

    // 输入字符串
    printf("请输入一个包含数字字符的字符串: ");
    fgets(str, sizeof(str), stdin);
    str[strcspn(str, "\n")] = '\0'; // 去除fgets读取的换行符

    // 调用函数进行转换
    int number = ConvertInt(str);

    // 输出转换后的整数
    printf("转换后的整数为: %d\n", number);

    return 0;
}