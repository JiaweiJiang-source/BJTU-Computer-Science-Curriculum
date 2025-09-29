#include <immintrin.h>  // 引入AVX指令库
#include <stdio.h>    
#include <stdlib.h> 
#include <time.h>      
#include <math.h>       

#define VECTOR_SIZE 16000000 

// 内存分配函数
// 作用：分配指定大小的内存
// 参数：size_t size - 需要分配的内存大小
// 返回：float* - 指向分配内存的指针
float* allocate_memory(size_t size) {
    float *ptr = (float*)malloc(size * sizeof(float));  // 分配内存
    if (ptr == NULL) {  
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE); 
    }
    return ptr;  
}

// 内存对齐分配函数
// 作用：分配指定对齐方式的内存
// 参数：size_t size - 需要分配的内存大小
//       size_t alignment - 对齐方式
// 返回：float* - 指向分配内存的指针
float* allocate_aligned_memory(size_t size, size_t alignment) {
    float *ptr = NULL;
    if (posix_memalign((void**)&ptr, alignment, size * sizeof(float)) != 0) {  // 分配对齐内存
        perror("Failed to allocate aligned memory");  
        exit(EXIT_FAILURE);  
    }
    return ptr;  // 返回指针
}

// 向量初始化函数
// 作用：初始化向量，将每个元素设为其索引值加1.0
// 参数：float* vec - 向量指针
//       size_t size - 向量大小
void initialize_vector(float* vec, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        vec[i] = (float)i + 1.0f;  // 初始化每个元素
    }
}

// 非AVX向量相乘函数
// 作用：逐元素相乘两个向量
// 参数：float* a - 第一个向量
//       float* b - 第二个向量
//       float* c - 结果向量
//       size_t size - 向量大小
void vector_mul(float* a, float* b, float* c, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        c[i] = a[i] * b[i];  // 逐一相乘
    }
}

// AVX-512向量相乘函数
// 作用：使用AVX-512逐元素相乘两个向量
// 参数：float* a - 第一个向量
//       float* b - 第二个向量
//       float* c - 结果向量
//       size_t size - 向量大小
void vector_mul_avx512(float* a, float* b, float* c, size_t size) {
    for (size_t i = 0; i < size; i += 16) {  // 每次处理16个元素
        __m512 av = _mm512_load_ps(&a[i]);  // 加载16个float到AVX寄存器
        __m512 bv = _mm512_load_ps(&b[i]);  // 加载16个float到AVX寄存器
        __m512 cv = _mm512_mul_ps(av, bv);  // 逐一相乘
        _mm512_store_ps(&c[i], cv);         // 存储结果
    }
}

// 结果比较函数
// 作用：比较两个结果向量是否相同
// 参数：float* vec1 - 第一个向量
//       float* vec2 - 第二个向量
//       size_t size - 向量大小
// 返回：int - 结果是否相同（1表示相同，0表示不同）
int compare_results(float* vec1, float* vec2, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        if (fabs(vec1[i] - vec2[i]) > 1e-5) {  // 比较每个元素
            return 0; // 结果不同
        }
    }
    return 1; // 结果相同
}

// 测量执行时间函数
// 作用：测量一个函数的执行时间
// 参数：void (*func)(float*, float*, float*, size_t) - 要测量的函数
//       float* a - 第一个向量
//       float* b - 第二个向量
//       float* c - 结果向量
//       size_t size - 向量大小
// 返回：double - 执行时间（秒）
double measure_time(void (*func)(float*, float*, float*, size_t), float* a, float* b, float* c, size_t size) {
    clock_t start = clock();  // 开始计时
    func(a, b, c, size);  // 执行函数
    clock_t end = clock();  // 结束计时
    return (double)(end - start) / CLOCKS_PER_SEC;  // 计算并返回执行时间
}

// 主函数
int main() {
    // 分配内存给向量
    float* vec_a = allocate_aligned_memory(VECTOR_SIZE, 64);
    float* vec_b = allocate_aligned_memory(VECTOR_SIZE, 64);
    float* vec_c_non_avx = allocate_aligned_memory(VECTOR_SIZE, 64);
    float* vec_c_avx512 = allocate_aligned_memory(VECTOR_SIZE, 64);

    // 初始化向量
    initialize_vector(vec_a, VECTOR_SIZE);
    initialize_vector(vec_b, VECTOR_SIZE);

    // 测量并打印非AVX向量相乘的时间
    double time_non_avx = measure_time(vector_mul, vec_a, vec_b, vec_c_non_avx, VECTOR_SIZE);
    printf("Non-AVX Time: %f seconds\n", time_non_avx);

    // 测量并打印AVX-512向量相乘的时间
    double time_avx512 = measure_time(vector_mul_avx512, vec_a, vec_b, vec_c_avx512, VECTOR_SIZE);
    printf("AVX-512 Time: %f seconds\n", time_avx512);

    // 比较结果
    if (compare_results(vec_c_non_avx, vec_c_avx512, VECTOR_SIZE)) {
        printf("Results match.\n");
    } else {
        printf("Results do not match!\n");
    }

    // 释放分配的内存
    free(vec_a);
    free(vec_b);
    free(vec_c_non_avx);
    free(vec_c_avx512);

    return 0; 
}