#include <stdio.h>   
#include <stdlib.h>   
#include <time.h>     

#define VECTOR_SIZE 16000000  

// 函数：allocate_memory
// 作用：分配指定大小的内存
// 参数：size_t size - 需要分配的内存大小
// 返回：float* - 指向分配内存的指针
float* allocate_memory(size_t size) {
    float *ptr = (float*)malloc(size * sizeof(float));  // 分配内存
    if (ptr == NULL) {  // 检查内存分配是否成功
        perror("Failed to allocate memory");  
        exit(EXIT_FAILURE);  
    }
    return ptr;  
}

// 函数：initialize_vector
// 作用：初始化向量，将每个元素设为其索引值加1.0
// 参数：float* vec - 向量指针
//       size_t size - 向量大小
void initialize_vector(float* vec, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        vec[i] = (float)i + 1.0f;  // 初始化每个元素
    }
}

// 函数：vector_mul
// 作用：向量元素逐一相乘
// 参数：float* a - 第一个向量
//       float* b - 第二个向量
//       float* c - 结果向量
//       size_t size - 向量大小
void vector_mul(float* a, float* b, float* c, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        c[i] = a[i] * b[i];  // 逐一相乘
    }
}

// 主函数
int main() {
    // 分配内存
    float* vec_a = allocate_memory(VECTOR_SIZE);  
    float* vec_b = allocate_memory(VECTOR_SIZE); 
    float* vec_c = allocate_memory(VECTOR_SIZE);  
	
    // 初始化向量
    initialize_vector(vec_a, VECTOR_SIZE);  
    initialize_vector(vec_b, VECTOR_SIZE);  

    clock_t start = clock();  // 开始计时
    vector_mul(vec_a, vec_b, vec_c, VECTOR_SIZE);  // 进行向量相乘运算
    clock_t end = clock();  // 结束计时

    double time_taken = (double)(end - start) / CLOCKS_PER_SEC;  // 计算所用时间
    printf("Non-AVX Time: %f seconds\n", time_taken);  

    //释放内存
    free(vec_a);  
    free(vec_b); 
    free(vec_c);  

    return 0;  
}