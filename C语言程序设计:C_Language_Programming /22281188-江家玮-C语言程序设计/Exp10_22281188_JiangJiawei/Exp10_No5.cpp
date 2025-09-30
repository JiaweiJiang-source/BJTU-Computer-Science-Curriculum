#include <stdio.h>

// 绘制简易直方图的函数
void DrawHistogram(int nArray[], int nCount, char ch) {
    for (int i = 0; i < nCount; i++) {
        // 打印字符ch，数量为nArray[i]
        for (int j = 0; j < nArray[i]; j++) {
            printf("%c", ch);
        }
        // 打印对应的数值
        printf(" %d\n", nArray[i]);
    }
}

// 统计数据在各个区间中出现的项数的函数
void CountRangeItems(double dArray[], int nDataItems, int nArray[], int nIntervals, double intervals[]) {
    // 初始化区间计数数组
    for (int i = 0; i < nIntervals; i++) {
        nArray[i] = 0;
    }

    // 遍历数据数组，统计每个数据所在的区间
    for (int i = 0; i < nDataItems; i++) {
        double value = dArray[i];
        int j = 0;

        // 找到数据所在的区间
        while (j < nIntervals - 1 && value > intervals[j]) {
            j++;
        }

        // 如果数据在最后一个区间（大于所有区间上限）
        if (j == nIntervals - 1 && value > intervals[j - 1]) {
            nArray[j]++;
        }
        else {
            nArray[j]++;
        }
    }
}

int main() {
    // 示例数据：身高数据
    double dArray[] = { 155.0, 162.0, 168.0, 172.0, 176.0, 180.0, 164.0, 173.0, 178.0, 185.0 };
    int nDataItems = sizeof(dArray) / sizeof(dArray[0]);

    // 定义区间：160以下, 160-165, 166-170, 171-175, 175以上
    int nIntervals = 5;
    double intervals[] = { 160.0, 165.0, 170.0, 175.0, 180.0 };

    // 区间统计结果数组
    int nArray[5];

    // 调用函数统计区间数据
    CountRangeItems(dArray, nDataItems, nArray, nIntervals, intervals);

    // 输出统计结果
    printf("区间统计结果:\n");
    for (int i = 0; i < nIntervals; i++) {
        printf("区间 %d: %d 人\n", i + 1, nArray[i]);
    }

    // 绘制直方图
    printf("\n直方图:\n");
    DrawHistogram(nArray, nIntervals, '*');

    return 0;
}