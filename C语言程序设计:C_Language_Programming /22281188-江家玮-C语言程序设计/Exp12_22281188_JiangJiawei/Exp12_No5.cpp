#include <stdio.h>

// 折半查找函数
int search(int* pArray, int n, int x) {
    int low = 0, high = n - 1;

    // 二分查找
    while (low <= high) {
        int mid = (low + high) / 2;

        if (pArray[mid] == x) {
            return 1; // 找到目标数，返回1
        }
        else if (pArray[mid] < x) {
            low = mid + 1; // 目标数在右半部分
        }
        else {
            high = mid - 1; // 目标数在左半部分
        }
    }

    return 0; // 未找到目标数，返回0
}

int main() {
    int sortedArray[10]; // 已排序的数组
    int x;               // 待查找的数

    // 输入10个已排序的整数
    printf("请输入10个已排序的整数（从小到大）:\n");
    for (int i = 0; i < 10; i++) {
        scanf_s("%d", &sortedArray[i]);
    }

    // 输入待查找的数
    printf("请输入待查找的数: ");
    scanf_s("%d", &x);

    // 调用search函数进行查找
    int result = search(sortedArray, 10, x);

    // 输出结果
    if (result == 1) {
        printf("数 %d 存在于数组中。\n", x);
    }
    else {
        printf("数 %d 不存在于数组中。\n", x);
    }

    return 0;
}