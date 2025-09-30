#include <stdio.h>
#include <math.h>

#define MAX_SIZE 100

// (1) 读入若干实数的功能
int ReadRealData(double dArray[], int nCount) {
    int actualCount = 0;
    printf("请输入 %d 个实数：\n", nCount);
    for (int i = 0; i < nCount; i++) {
        if (scanf_s("%lf", &dArray[i]) == 1) {  // 判断是否成功读入一个实数
            actualCount++;
        }
        else {
            break;  // 如果输入无效，结束输入
        }
    }
    return actualCount;  // 返回实际成功读取的个数
}

// (2) 查找最小元素的位置
int FindMinElement(double dArray[], int nCount) {
    int minIndex = 0;
    for (int i = 1; i < nCount; i++) {
        if (dArray[i] < dArray[minIndex]) {
            minIndex = i;
        }
    }
    return minIndex;
}

// (3) 查找最大元素的位置
int FindMaxElement(double dArray[], int nCount) {
    int maxIndex = 0;
    for (int i = 1; i < nCount; i++) {
        if (dArray[i] > dArray[maxIndex]) {
            maxIndex = i;
        }
    }
    return maxIndex;
}

// (4) 计算平均值
double Average(double dArray[], int nCount) {
    double sum = 0;
    for (int i = 0; i < nCount; i++) {
        sum += dArray[i];
    }
    return sum / nCount;  // 返回平均值
}

// (5) 计算方差
double Variance(double dArray[], int nCount) {
    double avg = Average(dArray, nCount);
    double varianceSum = 0;
    for (int i = 0; i < nCount; i++) {
        varianceSum += (dArray[i] - avg) * (dArray[i] - avg);
    }
    return varianceSum / nCount;  // 返回方差
}

// 测试函数
void TestFunctions() {
    double dArray[MAX_SIZE];
    int nCount;

    // 获取用户输入的元素个数
    printf("请输入数据集的元素个数：");
    scanf_s("%d", &nCount);

    // 读入数据
    int actualCount = ReadRealData(dArray, nCount);
    printf("成功读取 %d 个数据。\n", actualCount);

    // 计算最小值和最大值的下标
    int minIndex = FindMinElement(dArray, actualCount);
    int maxIndex = FindMaxElement(dArray, actualCount);

    printf("最小值位置: %d, 最小值: %.2lf\n", minIndex, dArray[minIndex]);
    printf("最大值位置: %d, 最大值: %.2lf\n", maxIndex, dArray[maxIndex]);

    // 计算平均值和方差
    double avg = Average(dArray, actualCount);
    double var = Variance(dArray, actualCount);

    printf("平均值: %.2lf\n", avg);
    printf("方差: %.2lf\n", var);
}

int main() {
    // 测试函数
    TestFunctions();
    return 0;
}
