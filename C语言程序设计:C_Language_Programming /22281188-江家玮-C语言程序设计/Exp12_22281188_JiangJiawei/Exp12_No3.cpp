#include <stdio.h>

// 使用数组实现
void action1(int dArray[], int n) {
    int minIndex = 0, maxIndex = 0;

    // 找到最小值和最大值的索引
    for (int i = 1; i < n; i++) {
        if (dArray[i] < dArray[minIndex]) {
            minIndex = i;
        }
        if (dArray[i] > dArray[maxIndex]) {
            maxIndex = i;
        }
    }

    // 交换最小值和第一个数
    int temp = dArray[0];
    dArray[0] = dArray[minIndex];
    dArray[minIndex] = temp;

    // 交换最大值和最后一个数
    temp = dArray[n - 1];
    dArray[n - 1] = dArray[maxIndex];
    dArray[maxIndex] = temp;
}

// 使用指针实现
void action2(int* pInt, int n) {
    int* minPtr = pInt, * maxPtr = pInt;

    // 找到最小值和最大值的指针
    for (int i = 1; i < n; i++) {
        if (*(pInt + i) < *minPtr) {
            minPtr = pInt + i;
        }
        if (*(pInt + i) > *maxPtr) {
            maxPtr = pInt + i;
        }
    }

    // 交换最小值和第一个数
    int temp = *pInt;
    *pInt = *minPtr;
    *minPtr = temp;

    // 交换最大值和最后一个数
    temp = *(pInt + n - 1);
    *(pInt + n - 1) = *maxPtr;
    *maxPtr = temp;
}

int main() {
    int dArray[10];

    // 输入10个整数
    printf("请输入10个整数:\n");
    for (int i = 0; i < 10; i++) {
        scanf_s("%d", &dArray[i]);
    }

    // 使用数组实现
    action1(dArray, 10);
    printf("\n使用数组实现后的结果:\n");
    for (int i = 0; i < 10; i++) {
        printf("%d ", dArray[i]);
    }
    printf("\n");

    // 恢复原始数组
    printf("\n请再次输入10个整数:\n");
    for (int i = 0; i < 10; i++) {
        scanf_s("%d", &dArray[i]);
    }

    // 使用指针实现
    action2(dArray, 10);
    printf("\n使用指针实现后的结果:\n");
    for (int i = 0; i < 10; i++) {
        printf("%d ", dArray[i]);
    }
    printf("\n");

    return 0;
}