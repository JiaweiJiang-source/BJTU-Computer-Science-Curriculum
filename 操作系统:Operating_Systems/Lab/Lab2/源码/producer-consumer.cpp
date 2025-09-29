#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h> 

#define BUFFER_SIZE 5

sem_t empty, full, mutex;

typedef int buffer_item;

buffer_item buffer[BUFFER_SIZE];

int in, out;
int id = 0;

int insert_item(buffer_item item) {
    buffer[out] = item;
    out = (out + 1) % BUFFER_SIZE;
    return 0;
}

int remove_item(buffer_item *item) {
    *item = buffer[in];
    in = (in + 1) % BUFFER_SIZE;
    return 0;
}

void *producer(void* param) {
    uintptr_t threadid = (uintptr_t)param; // 使用 uintptr_t
    while (1) {
        sem_wait(&empty);
        sem_wait(&mutex);
        insert_item(id);
        sleep(6);
        printf("ThreadId %lu : Producer produce product %d \n", (unsigned long)threadid, id); // 修改格式
        id++;
        sem_post(&mutex);
        sem_post(&full);
    }
}

void *consumer(void* param) {
    uintptr_t threadid = (uintptr_t)param; // 使用 uintptr_t
    while (1) {
        sem_wait(&full);
        sem_wait(&mutex);
        int item;
        remove_item(&item);
        sleep(3);
        printf("ThreadId %lu : Consumer consume product %d \n", (unsigned long)threadid, item); // 修改格式
        sem_post(&mutex);
        sem_post(&empty);
    }
}

int main() {
    pthread_t tid[4];
    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    in = out = 0;

    pthread_create(&tid[0], NULL, consumer, (void*)0);
    pthread_create(&tid[1], NULL, producer, (void*)1);
    pthread_create(&tid[2], NULL, consumer, (void*)2);
    pthread_create(&tid[3], NULL, producer, (void*)3);

    int c = 0;
    while (1) {
        c = getchar();
        if (c == 'q' || c == 'Q') {
            for (int i = 0; i < 4; ++i) {
                pthread_cancel(tid[i]);
            }
            break;
        }
    }

    sem_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);
    return 0;
}
