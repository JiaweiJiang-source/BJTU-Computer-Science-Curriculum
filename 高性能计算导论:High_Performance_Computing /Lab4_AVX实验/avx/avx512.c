#include <immintrin.h>  // ����AVXָ���
#include <stdio.h>      
#include <stdlib.h>     
#include <time.h>       

#define VECTOR_SIZE 16000000  

// ������allocate_aligned_memory
// ���ã�����ָ�����뷽ʽ���ڴ�
// ������size_t size - ��Ҫ������ڴ��С
//       size_t alignment - ���뷽ʽ
// ���أ�float* - ָ������ڴ��ָ��
float* allocate_aligned_memory(size_t size, size_t alignment) {
    float *ptr = NULL;
    if (posix_memalign((void**)&ptr, alignment, size * sizeof(float)) != 0) {  // ��������ڴ�
        perror("Failed to allocate aligned memory");  
        exit(EXIT_FAILURE);  
    }
    return ptr; 
}

// ������initialize_vector
// ���ã���ʼ����������ÿ��Ԫ����Ϊ������ֵ��1.0
// ������float* vec - ����ָ��
//       size_t size - ������С
void initialize_vector(float* vec, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        vec[i] = (float)i + 1.0f;  // ��ʼ��ÿ��Ԫ��
    }
}

// ������vector_mul_avx512
// ���ã�ʹ��AVX-512��������Ԫ����һ���
// ������float* a - ��һ������
//       float* b - �ڶ�������
//       float* c - �������
//       size_t size - ������С
void vector_mul_avx512(float* a, float* b, float* c, size_t size) {
    for (size_t i = 0; i < size; i += 16) {  // ÿ�δ���16��Ԫ��
        __m512 av = _mm512_load_ps(&a[i]);  // ����16��float��AVX�Ĵ���
        __m512 bv = _mm512_load_ps(&b[i]);  // ����16��float��AVX�Ĵ���
        __m512 cv = _mm512_mul_ps(av, bv);  // ��һ���
        _mm512_store_ps(&c[i], cv);         // �洢���
    }
}

// ������
int main() {
    //�����ڴ�
    float* vec_a = allocate_aligned_memory(VECTOR_SIZE, 64);  
    float* vec_b = allocate_aligned_memory(VECTOR_SIZE, 64);  
    float* vec_c = allocate_aligned_memory(VECTOR_SIZE, 64); 
	
    // ��ʼ������
    initialize_vector(vec_a, VECTOR_SIZE);  
    initialize_vector(vec_b, VECTOR_SIZE); 

    clock_t start = clock();  // ��ʼ��ʱ
    vector_mul_avx512(vec_a, vec_b, vec_c, VECTOR_SIZE);  // ���������������
    clock_t end = clock();  // ������ʱ

    double time_taken = (double)(end - start) / CLOCKS_PER_SEC;  // ��������ʱ��
    printf("AVX-512 Time: %f seconds\n", time_taken); 

    //�ͷ��ڴ�
    free(vec_a);  
    free(vec_b);  
    free(vec_c);  

    return 0;  
}