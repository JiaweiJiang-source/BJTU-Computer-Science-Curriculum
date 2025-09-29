#include <immintrin.h>  // ����AVXָ���
#include <stdio.h>      
#include <stdlib.h>     
#include <time.h>       
#include <math.h>       

#define N 512 

// ��ʼ��������
// ���ã������ֵ��ʼ������
// ������float* matrix - ����ָ��
//       size_t size - �����С
void initialize_matrix(float* matrix, size_t size) {
    for (size_t i = 0; i < size * size; ++i) {
        matrix[i] = (float)(rand() % 100) / 10.0f;  // ��ʼ������Ԫ��Ϊ0��9.9֮��������
    }
}

// ʹ��AVX-512���о���˷�
// ���ã�ʹ��AVX-512ָ����о���˷�
// ������float* a - ��һ������
//       float* b - �ڶ�������
//       float* c - �������
//       size_t size - �����С
void matrix_multiply_avx512(float* a, float* b, float* c, size_t size) {
    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < size; j++) {
            __m512 sum = _mm512_setzero_ps();  // ��ʼ��AVX�Ĵ���sumΪ0
            for (size_t k = 0; k < size; k += 16) {  // ÿ�δ���16��Ԫ��
                __m512 va = _mm512_load_ps(&a[i * size + k]);  // ����16��float��AVX�Ĵ���
                __m512 vb = _mm512_load_ps(&b[j * size + k]);  // ����16��float��AVX�Ĵ���
                sum = _mm512_fmadd_ps(va, vb, sum);  // ִ�г˼�����
            }
            c[i * size + j] = _mm512_reduce_add_ps(sum);  // ��Ͳ��洢���
        }
    }
}

// ��ʹ��AVX�ľ���˷�
// ���ã���Ԫ�������������
// ������float* a - ��һ������
//       float* b - �ڶ�������
//       float* c - �������
//       size_t size - �����С
void matrix_multiply_non_avx(float* a, float* b, float* c, size_t size) {
    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < size; j++) {
            float sum = 0.0;
            for (size_t k = 0; k < size; k++) {
                sum += a[i * size + k] * b[k * size + j];  // ��Ԫ����˲��ۼ�
            }
            c[i * size + j] = sum;  // �洢���
        }
    }
}

// �Ƚ����������Ƿ���ͬ
// ���ã��Ƚ����������Ԫ���Ƿ���ͬ
// ������float* m1 - ��һ������
//       float* m2 - �ڶ�������
//       size_t size - �����С
// ���أ�int - ����Ƿ���ͬ��1��ʾ��ͬ��0��ʾ��ͬ��
int compare_matrices(float* m1, float* m2, size_t size) {
    for (size_t i = 0; i < size * size; ++i) {
        if (fabs(m1[i] - m2[i]) > 1e-5) {  // �Ƚ�ÿ��Ԫ��
            return 0;  // ������ͬ
        }
    }
    return 1;  // ������ͬ
}

// ��������ִ��ʱ��
// ���ã�����һ��������ִ��ʱ��
// ������void (*func)(float*, float*, float*, size_t) - Ҫ�����ĺ���
//       float* a - ��һ������
//       float* b - �ڶ�������
//       float* c - �������
//       size_t size - �����С
// ���أ�double - ִ��ʱ�䣨�룩
double measure_time(void (*func)(float*, float*, float*, size_t), float* a, float* b, float* c, size_t size) {
    clock_t start = clock();  // ��ʼ��ʱ
    func(a, b, c, size);  // ִ�к���
    clock_t end = clock();  // ������ʱ
    return (double)(end - start) / CLOCKS_PER_SEC;  // ���㲢����ִ��ʱ��
}

// ������
int main() {
    srand(time(NULL)); 
    
    //�����ڴ�
    float* matrix_a = (float*)aligned_alloc(64, N * N * sizeof(float));  
    float* matrix_b = (float*)aligned_alloc(64, N * N * sizeof(float));  
    float* matrix_c_avx = (float*)aligned_alloc(64, N * N * sizeof(float));  
    float* matrix_c_non_avx = (float*)aligned_alloc(64, N * N * sizeof(float));  

    if (!matrix_a || !matrix_b || !matrix_c_avx || !matrix_c_non_avx) {
        perror("Failed to allocate memory");  // ����ڴ�����Ƿ�ɹ�
        return EXIT_FAILURE;  
    }

    initialize_matrix(matrix_a, N);  
    initialize_matrix(matrix_b, N);  

    // ��������ӡAVX-512����˷���ʱ��
    double time_avx = measure_time(matrix_multiply_avx512, matrix_a, matrix_b, matrix_c_avx, N);
    printf("AVX-512 Time: %f seconds\n", time_avx);

    // ��������ӡ��AVX����˷���ʱ��
    double time_non_avx = measure_time(matrix_multiply_non_avx, matrix_a, matrix_b, matrix_c_non_avx, N);
    printf("Non-AVX Time: %f seconds\n", time_non_avx);

    // �Ƚ�AVX-512�ͷ�AVX����˷��Ľ��
    if (compare_matrices(matrix_c_avx, matrix_c_non_avx, N)) {
        printf("Results match.\n");
    } else {
        printf("Results do not match!\n");
    }

    // ������ٱ�
    double speedup = time_non_avx / time_avx;
    printf("Speedup: %f\n", speedup);

    // �ͷŷ�����ڴ�
    free(matrix_a);
    free(matrix_b);
    free(matrix_c_avx);
    free(matrix_c_non_avx);

    return 0;  // ���سɹ�״̬
}