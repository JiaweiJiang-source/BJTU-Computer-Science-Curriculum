#include <immintrin.h>  // ����AVXָ���
#include <stdio.h>    
#include <stdlib.h> 
#include <time.h>      
#include <math.h>       

#define VECTOR_SIZE 16000000 

// �ڴ���亯��
// ���ã�����ָ����С���ڴ�
// ������size_t size - ��Ҫ������ڴ��С
// ���أ�float* - ָ������ڴ��ָ��
float* allocate_memory(size_t size) {
    float *ptr = (float*)malloc(size * sizeof(float));  // �����ڴ�
    if (ptr == NULL) {  
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE); 
    }
    return ptr;  
}

// �ڴ������亯��
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
    return ptr;  // ����ָ��
}

// ������ʼ������
// ���ã���ʼ����������ÿ��Ԫ����Ϊ������ֵ��1.0
// ������float* vec - ����ָ��
//       size_t size - ������С
void initialize_vector(float* vec, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        vec[i] = (float)i + 1.0f;  // ��ʼ��ÿ��Ԫ��
    }
}

// ��AVX������˺���
// ���ã���Ԫ�������������
// ������float* a - ��һ������
//       float* b - �ڶ�������
//       float* c - �������
//       size_t size - ������С
void vector_mul(float* a, float* b, float* c, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        c[i] = a[i] * b[i];  // ��һ���
    }
}

// AVX-512������˺���
// ���ã�ʹ��AVX-512��Ԫ�������������
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

// ����ȽϺ���
// ���ã��Ƚ�������������Ƿ���ͬ
// ������float* vec1 - ��һ������
//       float* vec2 - �ڶ�������
//       size_t size - ������С
// ���أ�int - ����Ƿ���ͬ��1��ʾ��ͬ��0��ʾ��ͬ��
int compare_results(float* vec1, float* vec2, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        if (fabs(vec1[i] - vec2[i]) > 1e-5) {  // �Ƚ�ÿ��Ԫ��
            return 0; // �����ͬ
        }
    }
    return 1; // �����ͬ
}

// ����ִ��ʱ�亯��
// ���ã�����һ��������ִ��ʱ��
// ������void (*func)(float*, float*, float*, size_t) - Ҫ�����ĺ���
//       float* a - ��һ������
//       float* b - �ڶ�������
//       float* c - �������
//       size_t size - ������С
// ���أ�double - ִ��ʱ�䣨�룩
double measure_time(void (*func)(float*, float*, float*, size_t), float* a, float* b, float* c, size_t size) {
    clock_t start = clock();  // ��ʼ��ʱ
    func(a, b, c, size);  // ִ�к���
    clock_t end = clock();  // ������ʱ
    return (double)(end - start) / CLOCKS_PER_SEC;  // ���㲢����ִ��ʱ��
}

// ������
int main() {
    // �����ڴ������
    float* vec_a = allocate_aligned_memory(VECTOR_SIZE, 64);
    float* vec_b = allocate_aligned_memory(VECTOR_SIZE, 64);
    float* vec_c_non_avx = allocate_aligned_memory(VECTOR_SIZE, 64);
    float* vec_c_avx512 = allocate_aligned_memory(VECTOR_SIZE, 64);

    // ��ʼ������
    initialize_vector(vec_a, VECTOR_SIZE);
    initialize_vector(vec_b, VECTOR_SIZE);

    // ��������ӡ��AVX������˵�ʱ��
    double time_non_avx = measure_time(vector_mul, vec_a, vec_b, vec_c_non_avx, VECTOR_SIZE);
    printf("Non-AVX Time: %f seconds\n", time_non_avx);

    // ��������ӡAVX-512������˵�ʱ��
    double time_avx512 = measure_time(vector_mul_avx512, vec_a, vec_b, vec_c_avx512, VECTOR_SIZE);
    printf("AVX-512 Time: %f seconds\n", time_avx512);

    // �ȽϽ��
    if (compare_results(vec_c_non_avx, vec_c_avx512, VECTOR_SIZE)) {
        printf("Results match.\n");
    } else {
        printf("Results do not match!\n");
    }

    // �ͷŷ�����ڴ�
    free(vec_a);
    free(vec_b);
    free(vec_c_non_avx);
    free(vec_c_avx512);

    return 0; 
}