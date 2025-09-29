#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#define N 5

int count = 0, a = 5, b = 5;
int r[N] = {0, 1, 2, 3, 4};
sem_t wmutex, rmutex;

void delay() {
    int time = rand() % 10 + 1; // 随机使程序睡眠0点几秒
    usleep(time * 100000);
}

void *Reader(void *arg) { // 修改为返回 void*
    int i = *(int *)arg;
    while (a > 0) {
        a--;
        delay();
        sem_wait(&rmutex);
        if (count == 0)
            sem_wait(&wmutex);
        count++;
        sem_post(&rmutex);

        printf("Reader%d is reading!\n", i);
        printf("Reader%d reads end!\n", i);

        sem_wait(&rmutex);
        count--;
        if (count == 0)
            sem_post(&wmutex);
        sem_post(&rmutex);
    }
    return NULL; // 添加返回
}

void *Writer(void *arg) { // 修改为接受 void* 类型参数
    while (b > 0) {
        b--;
        delay();
        sem_wait(&wmutex);

        printf("writer is writing!\n");
        printf("writer writes end!\n");

        sem_post(&wmutex);
    }
    return NULL; // 添加返回
}

int main() {
    int i;
    pthread_t writer, reader[N];
    srand((unsigned int)time(NULL));

    sem_init(&wmutex, 0, 1); // 互斥锁初始化
    sem_init(&rmutex, 0, 1);

    for (i = 0; i < N; i++) { // 创建线程
        pthread_create(&reader[i], NULL, Reader, &r[i]);
    }

    pthread_create(&writer, NULL, Writer, NULL);

    for (i = 0; i < N; i++) { // 等待所有读者线程结束
        pthread_join(reader[i], NULL);
    }
    pthread_join(writer, NULL); // 等待写者线程结束

    sem_destroy(&rmutex); // 互斥锁的销毁
    sem_destroy(&wmutex);

    return 0;
}
