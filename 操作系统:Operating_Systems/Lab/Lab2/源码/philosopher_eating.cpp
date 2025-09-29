#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#define NUM 5
int ID[NUM] = {0, 1, 2, 3, 4};
sem_t sem_chopsticks[NUM];
sem_t sem_eaters;
int eaters_num = 0;

void sem_signal_init() {
    for (int i = 0; i < NUM; i++) {
        if (sem_init(&sem_chopsticks[i], 0, 1) == -1) {
            perror("oops: sem_init error!");
            exit(1);
        }
    }
    if (sem_init(&sem_eaters, 0, NUM - 1) == -1) {
        perror("oops: sem_init error!");
        exit(1);
    }
}

void *philosopher(void *ptid) { // 修改为返回 void*
    int pthread_id = *(int *)ptid % NUM;
    printf("%d philosopher is thinking...\n", pthread_id);
    sem_wait(&sem_eaters);
    sem_wait(&sem_chopsticks[pthread_id]);
    printf("%d philosopher takes chopstick %d...\n", pthread_id, pthread_id);
    sem_wait(&sem_chopsticks[(pthread_id + 1) % NUM]);
    printf("%d philosopher takes chopstick %d...\n", pthread_id, (pthread_id + 1) % NUM);
    printf("%d philosopher is eating, %d philosopher had already dined.\n", pthread_id, eaters_num);
    sem_post(&sem_chopsticks[(pthread_id + 1) % NUM]);
    sem_post(&sem_chopsticks[pthread_id]);
    sem_post(&sem_eaters);
    eaters_num++;
    printf("%d philosopher had dined, by now %d philosopher had already dined.\n", pthread_id, eaters_num);
    return NULL; // 添加返回
}

int main() {
    for (int l = 0; l < 1000; ++l) {
        printf("**********************%d times try ******************************\n", l + 1);
        pthread_t philosopher_threads[NUM];
        sem_signal_init();
        for (int i = 0; i < NUM; i++) {
            printf("%d times\n", i);
            if (pthread_create(&philosopher_threads[i], NULL, philosopher, &ID[i]) != 0) {
                perror("oops: pthread_create error!");
                exit(1);
            }
        }

        for (int j = 0; j < NUM; j++) {
            pthread_join(philosopher_threads[j], NULL);
        }
        sem_destroy(&sem_eaters);
        for (int k = 0; k < NUM; ++k) {
            sem_destroy(&sem_chopsticks[k]);
        }
        eaters_num = 0;
    }

    return 0;
}
