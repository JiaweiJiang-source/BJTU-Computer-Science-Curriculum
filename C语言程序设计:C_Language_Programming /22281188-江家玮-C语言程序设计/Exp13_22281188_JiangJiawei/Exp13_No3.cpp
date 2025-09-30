#include <stdio.h>
#include <stdlib.h>

// 使用数组写法判断矩阵是否对称
int isSymmetricArray(int matrix[][10], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] != matrix[j][i]) {
                return 0; // 不对称，返回0
            }
        }
    }
    return 1; // 对称，返回1
}

// 使用指针写法判断矩阵是否对称
int isSymmetricPointer(int* matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (*(matrix + i * n + j) != *(matrix + j * n + i)) {
                return 0; // 不对称，返回0
            }
        }
    }
    return 1; // 对称，返回1
}

int main() {
    int n; // 矩阵的阶数
    int matrix[10][10]; // 定义矩阵，假设最大阶数为10

    // 输入矩阵的阶数
    printf("请输入矩阵的阶数n (n <= 10): ");
    scanf_s("%d", &n);

    // 输入矩阵的元素
    printf("请输入%d阶矩阵的元素:\n", n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            scanf_s("%d", &matrix[i][j]);
        }
    }

    // 使用数组写法判断矩阵是否对称
    if (isSymmetricArray(matrix, n)) {
        printf("使用数组写法判断：矩阵是对称的。\n");
    }
    else {
        printf("使用数组写法判断：矩阵不是对称的。\n");
    }

    // 使用指针写法判断矩阵是否对称
    if (isSymmetricPointer(&matrix[0][0], n)) {
        printf("使用指针写法判断：矩阵是对称的。\n");
    }
    else {
        printf("使用指针写法判断：矩阵不是对称的。\n");
    }

    return 0;
}