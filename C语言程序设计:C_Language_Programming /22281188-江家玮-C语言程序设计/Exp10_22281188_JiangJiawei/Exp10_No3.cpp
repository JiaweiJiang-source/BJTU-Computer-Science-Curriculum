#include <stdio.h>

// 排序函数，排序数组的前 n 个元素，按从大到小排序
void Sort(int nArray[], int n) {
    // 冒泡排序：从大到小排序前 8 个元素
    for (int i = 0; i < 8 - 1; i++) {  // 遍历 8 次
        for (int j = 0; j < 8 - i - 1; j++) {  // 交换相邻元素
            if (nArray[j] < nArray[j + 1]) {  // 如果前一个小于后一个，交换
                int temp = nArray[j];
                nArray[j] = nArray[j + 1];
                nArray[j + 1] = temp;
            }
        }
    }
}

int main() {
    int nArray[10];

    // 从键盘输入 10 个整数
    printf("请输入 10 个整数：\n");
    for (int i = 0; i < 10; i++) {
        scanf_s("%d", &nArray[i]);
    }

    // 调用 Sort 函数对前 8 个元素进行排序
    Sort(nArray, 10);

    // 输出排序后的数组
    printf("排序后的数组：\n");
    for (int i = 0; i < 10; i++) {
        printf("%d ", nArray[i]);
    }
    printf("\n");

    return 0;
}
