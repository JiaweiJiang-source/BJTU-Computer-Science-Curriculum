#include <stdio.h>
#include <ctype.h> // 用于tolower函数

// 统计某字母在字符串中出现的次数（不区分大小写）
int count(const char str[], char c) {
    int count = 0;
    char lower_c = tolower(c); // 将待统计字母转换为小写

    // 遍历字符串
    for (int i = 0; str[i] != '\0'; i++) {
        // 将字符串中的字符转换为小写并比较
        if (tolower(str[i]) == lower_c) {
            count++;
        }
    }

    return count;
}

int main() {
    char str[100]; // 定义字符串数组
    char c;        // 定义待统计的字母

    // 输入字符串
    printf("请输入一个字符串: ");
    fgets(str, sizeof(str), stdin); // 使用fgets读取字符串，避免缓冲区溢出

    // 输入待统计的字母
    printf("请输入待统计的字母: ");
    scanf_s(" %c", &c); // 注意前面的空格，用于跳过换行符

    // 调用count函数统计字母出现的次数
    int result = count(str, c);

    // 输出结果
    printf("字母 '%c' 在字符串中出现的次数为: %d\n", c, result);

    return 0;
}