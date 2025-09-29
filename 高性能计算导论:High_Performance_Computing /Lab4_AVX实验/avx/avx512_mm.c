#include <stdio.h>   
#include <stdlib.h>   
#include <time.h>     

#define VECTOR_SIZE 16000000  

// ������allocate_memory
// ���ã�����ָ����С���ڴ�
// ������size_t size - ��Ҫ������ڴ��С
// ���أ�float* - ָ������ڴ��ָ��
float* allocate_memory(size_t size) {
    float *ptr = (float*)malloc(size * sizeof(float));  // �����ڴ�
    if (ptr == NULL) {  // ����ڴ�����Ƿ�ɹ�
        perror("Failed to allocate memory");  
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

// ������vector_mul
// ���ã�����Ԫ����һ���
// ������float* a - ��һ������
//       float* b - �ڶ�������
//       float* c - �������
//       size_t size - ������С
void vector_mul(float* a, float* b, float* c, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        c[i] = a[i] * b[i];  // ��һ���
    }
}

// ������
int main() {
    // �����ڴ�
    float* vec_a = allocate_memory(VECTOR_SIZE);  
    float* vec_b = allocate_memory(VECTOR_SIZE); 
    float* vec_c = allocate_memory(VECTOR_SIZE);  
	
    // ��ʼ������
    initialize_vector(vec_a, VECTOR_SIZE);  
    initialize_vector(vec_b, VECTOR_SIZE);  

    clock_t start = clock();  // ��ʼ��ʱ
    vector_mul(vec_a, vec_b, vec_c, VECTOR_SIZE);  // ���������������
    clock_t end = clock();  // ������ʱ

    double time_taken = (double)(end - start) / CLOCKS_PER_SEC;  // ��������ʱ��
    printf("Non-AVX Time: %f seconds\n", time_taken);  

    //�ͷ��ڴ�
    free(vec_a);  
    free(vec_b); 
    free(vec_c);  

    return 0;  
}