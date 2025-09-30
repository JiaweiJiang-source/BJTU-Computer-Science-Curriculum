#include <stdio.h>

// 交换两个数的函数，使用指针作为参数
void swap(int* a, int* b) {
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

int main() {
    int num1, num2;

    // 输入两个数
    printf("请输入第一个数: ");
    scanf_s("%d", &num1);
    printf("请输入第二个数: ");
    scanf_s("%d", &num2);

    // 输出交换前的值
    printf("\n交换前的值:\n");
    printf("num1 = %d, num2 = %d\n", num1, num2);

    // 调用swap函数交换两个数的值
    swap(&num1, &num2);

    // 输出交换后的值
    printf("\n交换后的值:\n");
    printf("num1 = %d, num2 = %d\n", num1, num2);

    return 0;
}