#include <immintrin.h>  // 引入AVX指令库
#include <stdio.h>      
#include <stdlib.h>     
#include <time.h>       

#define VECTOR_SIZE 16000000  

// 函数：allocate_aligned_memory
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

// 函数：vector_mul_avx512
// 作用：使用AVX-512进行向量元素逐一相乘
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

// 主函数
int main() {
    //分配内存
    float* vec_a = allocate_aligned_memory(VECTOR_SIZE, 64);  
    float* vec_b = allocate_aligned_memory(VECTOR_SIZE, 64);  
    float* vec_c = allocate_aligned_memory(VECTOR_SIZE, 64); 
	
    // 初始化向量
    initialize_vector(vec_a, VECTOR_SIZE);  
    initialize_vector(vec_b, VECTOR_SIZE); 

    clock_t start = clock();  // 开始计时
    vector_mul_avx512(vec_a, vec_b, vec_c, VECTOR_SIZE);  // 进行向量相乘运算
    clock_t end = clock();  // 结束计时

    double time_taken = (double)(end - start) / CLOCKS_PER_SEC;  // 计算所用时间
    printf("AVX-512 Time: %f seconds\n", time_taken); 

    //释放内存
    free(vec_a);  
    free(vec_b);  
    free(vec_c);  

    return 0;  
}