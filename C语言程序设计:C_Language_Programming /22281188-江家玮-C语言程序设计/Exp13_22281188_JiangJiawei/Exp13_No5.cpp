#include <stdio.h>

// 查找二维矩阵的鞍点
void findSaddlePoint(int matrix[][10], int rows, int cols) {
    int found = 0; // 标记是否找到鞍点

    for (int i = 0; i < rows; i++) {
        int maxRow = matrix[i][0]; // 假设当前行的第一个元素为最大值
        int colIndex = 0;         // 最大值所在的列索引

        // 找出当前行的最大值及其列索引
        for (int j = 1; j < cols; j++) {
            if (matrix[i][j] > maxRow) {
                maxRow = matrix[i][j];
                colIndex = j;
            }
        }

        // 检查最大值是否为所在列的最小值
        int isSaddlePoint = 1; // 假设是鞍点
        for (int k = 0; k < rows; k++) {
            if (matrix[k][colIndex] < maxRow) {
                isSaddlePoint = 0; // 不是鞍点
                break;
            }
        }

        // 如果找到鞍点，打印并标记
        if (isSaddlePoint) {
            printf("鞍点位于第 %d 行，第 %d 列，值为 %d\n", i + 1, colIndex + 1, maxRow);
            found = 1;
        }
    }

    // 如果没有找到鞍点，打印相应信息
    if (!found) {
        printf("矩阵中没有鞍点。\n");
    }
}

int main() {
    int rows, cols;
    int matrix[10][10]; // 定义二维矩阵，假设最大维度为10x10

    // 输入矩阵的行数和列数
    printf("请输入矩阵的行数和列数（最大为10x10）: ");
    scanf_s("%d %d", &rows, &cols);

    // 输入矩阵的元素
    printf("请输入矩阵的元素:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            scanf_s("%d", &matrix[i][j]);
        }
    }

    // 查找鞍点
    findSaddlePoint(matrix, rows, cols);

    return 0;
}