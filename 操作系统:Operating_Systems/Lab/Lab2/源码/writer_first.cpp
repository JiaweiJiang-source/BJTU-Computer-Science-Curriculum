#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#define N 5

int readcount = 0, writecount = 0, a = 5, b = 2;
int r[N] = {0, 1, 2, 3, 4};
int w[N] = {0, 1};
sem_t wmutex, rmutex, mutex1, num;

void delay() {
    int time = rand() % 10 + 1; // 随机使程序睡眠0点几秒
    usleep(time * 100000);
}

void *Reader(void *arg) { // 修改为返回 void*
    int i = *(int *)arg;
    while (a > 0) {
        a--;
        delay(); // 延迟

        // 进入共享文件前的准备 
        sem_wait(&num); // 在无写者进程时进入 
        sem_wait(&rmutex); // 与其他读者进程互斥的访问 readcount 
        if (readcount == 0)
            sem_wait(&mutex1); // 与写者进程互斥的访问共享文件 
        readcount++;
        sem_post(&rmutex);
        sem_post(&num);

        // reader
        printf("Reader%d is reading!\n", i);
        printf("Reader%d reads end!\n", i);

        // 退出共享文件后的处理 
        sem_wait(&rmutex);
        readcount--;
        if (readcount == 0)
            sem_post(&mutex1);
        sem_post(&rmutex);
    }
    return NULL; // 添加返回
}

void *Writer(void *arg) { // 修改为接受 void* 类型参数
    int i = *(int *)arg;
    while (b > 0) {
        b--;
        delay();

        // 进入共享文件前的准备 
        sem_wait(&wmutex); // 保证多个写者进程能够互斥使用 writecount 
        writecount++;
        if (writecount == 1)
            sem_wait(&num); // 用于禁止读者进程 
        sem_post(&wmutex);

        // writer
        sem_wait(&mutex1); // 与其他所有进程互斥的访问共享文件
        printf("writer%d is writing!\n", i);
        printf("writer%d writes end!\n", i);
        sem_post(&mutex1);

        // 退出共享文件后的处理 
        sem_wait(&wmutex);
        writecount--;
        if (writecount == 0)
            sem_post(&num);
        sem_post(&wmutex);
    }
    return NULL; // 添加返回
}

int main() {
    int i;
    pthread_t writer[N], reader[N];
    srand((unsigned int)time(NULL));

    sem_init(&wmutex, 0, 1); // 互斥锁初始化 
    sem_init(&rmutex, 0, 1);
    sem_init(&mutex1, 0, 1);
    sem_init(&num, 0, 1);

    for (i = 0; i < 5; i++) { // 创建线程 
        pthread_create(&reader[i], NULL, Reader, &r[i]);
    }

    for (i = 0; i < 2; i++) { // 创建线程 
        pthread_create(&writer[i], NULL, Writer, &w[i]);
    }

    for (i = 0; i < 2; i++) { // 等待线程 
        pthread_join(writer[i], NULL);
    }

    for (i = 0; i < 5; i++) { // 等待线程 
        pthread_join(reader[i], NULL);
    }

    sem_destroy(&rmutex); // 互斥锁的销毁
    sem_destroy(&wmutex);
    sem_destroy(&mutex1);
    sem_destroy(&num);

    return 0;
}
