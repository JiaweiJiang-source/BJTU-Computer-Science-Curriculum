#include <stdio.h>
#include <math.h>
#include <time.h>

// 整数操作的幂运算
int int_power(int base, int exp) {
    int result = 1;
    for (int i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}

// 浮点操作的幂运算
double float_power(double base, int exp) {
    return pow(base, exp);
}

// 测试执行时间（分别计算整数和浮点操作的时间）
void benchmark(int int_frequency, int float_frequency, int runs) {
    int int_base = 2;
    int int_exp = 10;
    double float_base = 2.0;
    int float_exp = 10;

    clock_t start, end;
    double total_int_time = 0.0;
    double total_float_time = 0.0;

    for (int i = 0; i < runs; i++) {
        if (i % (int_frequency + float_frequency) < int_frequency) {
            // 执行整数操作
            start = clock();
            for (int j = 0; j < 100000; j++) { // 增加工作量
                int_power(int_base, int_exp);
            }
            end = clock();
            total_int_time += (double)(end - start) / CLOCKS_PER_SEC;
        }
        else {
            // 执行浮点操作
            start = clock();
            for (int j = 0; j < 100000; j++) { // 增加工作量
                float_power(float_base, float_exp);
            }
            end = clock();
            total_float_time += (double)(end - start) / CLOCKS_PER_SEC;
        }
    }

    // 计算加权平均运行时间
    double weighted_avg_time = ((total_int_time * int_frequency) + (total_float_time * float_frequency)) / (int_frequency + float_frequency);

    // 输出整数和浮点操作的总时间
    printf("Integer Method Total Time: %f seconds\n", total_int_time);
    printf("Float Method Total Time: %f seconds\n", total_float_time);
    printf("Weighted Average Time: %f seconds\n", weighted_avg_time);
}

int main() {
    int runs = 10000; // 运行次数

    // (0.5, 0.5) 频率
    printf("Frequency (0.5, 0.5):\n");
    benchmark(5, 5, runs);

    // (0.1, 0.9) 频率
    printf("\nFrequency (0.1, 0.9):\n");
    benchmark(1, 9, runs);

    // (0.9, 0.1) 频率
    printf("\nFrequency (0.9, 0.1):\n");
    benchmark(9, 1, runs);

    return 0;
}
