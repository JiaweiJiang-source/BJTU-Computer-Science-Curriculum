#include <immintrin.h>  // 引入AVX指令库
#include <stdio.h>      
#include <stdlib.h>     
#include <time.h>       
#include <math.h>       

#define N 512 

// 初始化矩阵函数
// 作用：用随机值初始化矩阵
// 参数：float* matrix - 矩阵指针
//       size_t size - 矩阵大小
void initialize_matrix(float* matrix, size_t size) {
    for (size_t i = 0; i < size * size; ++i) {
        matrix[i] = (float)(rand() % 100) / 10.0f;  // 初始化矩阵元素为0到9.9之间的随机数
    }
}

// 使用AVX-512进行矩阵乘法
// 作用：使用AVX-512指令集进行矩阵乘法
// 参数：float* a - 第一个矩阵
//       float* b - 第二个矩阵
//       float* c - 结果矩阵
//       size_t size - 矩阵大小
void matrix_multiply_avx512(float* a, float* b, float* c, size_t size) {
    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < size; j++) {
            __m512 sum = _mm512_setzero_ps();  // 初始化AVX寄存器sum为0
            for (size_t k = 0; k < size; k += 16) {  // 每次处理16个元素
                __m512 va = _mm512_load_ps(&a[i * size + k]);  // 加载16个float到AVX寄存器
                __m512 vb = _mm512_load_ps(&b[j * size + k]);  // 加载16个float到AVX寄存器
                sum = _mm512_fmadd_ps(va, vb, sum);  // 执行乘加运算
            }
            c[i * size + j] = _mm512_reduce_add_ps(sum);  // 求和并存储结果
        }
    }
}

// 不使用AVX的矩阵乘法
// 作用：逐元素相乘两个矩阵
// 参数：float* a - 第一个矩阵
//       float* b - 第二个矩阵
//       float* c - 结果矩阵
//       size_t size - 矩阵大小
void matrix_multiply_non_avx(float* a, float* b, float* c, size_t size) {
    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < size; j++) {
            float sum = 0.0;
            for (size_t k = 0; k < size; k++) {
                sum += a[i * size + k] * b[k * size + j];  // 逐元素相乘并累加
            }
            c[i * size + j] = sum;  // 存储结果
        }
    }
}

// 比较两个矩阵是否相同
// 作用：比较两个矩阵的元素是否相同
// 参数：float* m1 - 第一个矩阵
//       float* m2 - 第二个矩阵
//       size_t size - 矩阵大小
// 返回：int - 结果是否相同（1表示相同，0表示不同）
int compare_matrices(float* m1, float* m2, size_t size) {
    for (size_t i = 0; i < size * size; ++i) {
        if (fabs(m1[i] - m2[i]) > 1e-5) {  // 比较每个元素
            return 0;  // 矩阵不相同
        }
    }
    return 1;  // 矩阵相同
}

// 测量函数执行时间
// 作用：测量一个函数的执行时间
// 参数：void (*func)(float*, float*, float*, size_t) - 要测量的函数
//       float* a - 第一个矩阵
//       float* b - 第二个矩阵
//       float* c - 结果矩阵
//       size_t size - 矩阵大小
// 返回：double - 执行时间（秒）
double measure_time(void (*func)(float*, float*, float*, size_t), float* a, float* b, float* c, size_t size) {
    clock_t start = clock();  // 开始计时
    func(a, b, c, size);  // 执行函数
    clock_t end = clock();  // 结束计时
    return (double)(end - start) / CLOCKS_PER_SEC;  // 计算并返回执行时间
}

// 主函数
int main() {
    srand(time(NULL)); 
    
    //分配内存
    float* matrix_a = (float*)aligned_alloc(64, N * N * sizeof(float));  
    float* matrix_b = (float*)aligned_alloc(64, N * N * sizeof(float));  
    float* matrix_c_avx = (float*)aligned_alloc(64, N * N * sizeof(float));  
    float* matrix_c_non_avx = (float*)aligned_alloc(64, N * N * sizeof(float));  

    if (!matrix_a || !matrix_b || !matrix_c_avx || !matrix_c_non_avx) {
        perror("Failed to allocate memory");  // 检查内存分配是否成功
        return EXIT_FAILURE;  
    }

    initialize_matrix(matrix_a, N);  
    initialize_matrix(matrix_b, N);  

    // 测量并打印AVX-512矩阵乘法的时间
    double time_avx = measure_time(matrix_multiply_avx512, matrix_a, matrix_b, matrix_c_avx, N);
    printf("AVX-512 Time: %f seconds\n", time_avx);

    // 测量并打印非AVX矩阵乘法的时间
    double time_non_avx = measure_time(matrix_multiply_non_avx, matrix_a, matrix_b, matrix_c_non_avx, N);
    printf("Non-AVX Time: %f seconds\n", time_non_avx);

    // 比较AVX-512和非AVX矩阵乘法的结果
    if (compare_matrices(matrix_c_avx, matrix_c_non_avx, N)) {
        printf("Results match.\n");
    } else {
        printf("Results do not match!\n");
    }

    // 计算加速比
    double speedup = time_non_avx / time_avx;
    printf("Speedup: %f\n", speedup);

    // 释放分配的内存
    free(matrix_a);
    free(matrix_b);
    free(matrix_c_avx);
    free(matrix_c_non_avx);

    return 0;  // 返回成功状态
}