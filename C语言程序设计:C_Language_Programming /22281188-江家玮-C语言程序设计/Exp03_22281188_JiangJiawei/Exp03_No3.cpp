#include <stdio.h>

int main() {
    // 1) 以%c格式打印字符 'a'
    printf("1) %c\n", 'a');  // 输出字符 'a'

    // 2) 以%c格式打印 'a' + 3
    printf("2) %c\n", 'a' + 3);  // 输出 'd'

    // 3) 以%c格式打印 'A' + 32
    printf("3) %c\n", 'A' + 32);  // 输出 'a'

    // 4) 以%c格式打印 97
    printf("4) %c\n", 97);  // 输出 'a'

    // 5) 以%d格式打印 'a'
    printf("5) %d\n", 'a');  // 输出 97
    return 0;
}
