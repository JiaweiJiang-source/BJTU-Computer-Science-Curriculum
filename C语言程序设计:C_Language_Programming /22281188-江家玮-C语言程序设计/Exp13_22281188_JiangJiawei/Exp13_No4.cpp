#include <stdio.h>

// 打印杨辉三角
void printPascalTriangle(int n) {
    int triangle[100][100]; // 定义二维数组存储杨辉三角

    // 初始化杨辉三角的第一列和对角线为1
    for (int i = 0; i < n; i++) {
        triangle[i][0] = 1; // 第一列全为1
        triangle[i][i] = 1; // 对角线全为1
    }

    // 计算杨辉三角的其他元素
    for (int i = 2; i < n; i++) {
        for (int j = 1; j < i; j++) {
            triangle[i][j] = triangle[i - 1][j - 1] + triangle[i - 1][j];
        }
    }

    // 打印杨辉三角
    printf("杨辉三角（%d行）:\n", n);
    for (int i = 0; i < n; i++) {
        // 打印每行的元素
        for (int j = 0; j <= i; j++) {
            printf("%d ", triangle[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int n; // 杨辉三角的行数

    // 输入杨辉三角的行数
    printf("请输入杨辉三角的行数: ");
    scanf_s("%d", &n);

    // 打印杨辉三角
    printPascalTriangle(n);

    return 0;
}