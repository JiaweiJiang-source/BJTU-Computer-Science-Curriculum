#include <stdio.h>

// 逆序排列函数
void inverse(int* array, int nStart, int nInverseNumber) {
    int startIndex = nStart - 1; // 数组索引从0开始，nStart从1开始
    int endIndex = startIndex + nInverseNumber - 1; // 逆序部分的结束索引

    // 逆序排列
    while (startIndex < endIndex) {
        // 交换两个元素
        int temp = array[startIndex];
        array[startIndex] = array[endIndex];
        array[endIndex] = temp;

        // 移动索引
        startIndex++;
        endIndex--;
    }
}

int main() {
    int array[10]; // 定义数组，存储10个整数
    int nStart, nInverseNumber;

    // 输入10个整数
    printf("请输入10个整数:\n");
    for (int i = 0; i < 10; i++) {
        scanf_s("%d", &array[i]);
    }

    // 输入起始位置和逆序的个数
    printf("请输入起始位置nStart (1-10): ");
    scanf_s("%d", &nStart);
    printf("请输入逆序的个数nInverseNumber: ");
    scanf_s("%d", &nInverseNumber);

    // 调用函数进行逆序排列
    inverse(array, nStart, nInverseNumber);

    // 输出新的数组
    printf("逆序排列后的数组为:\n");
    for (int i = 0; i < 10; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    return 0;
}