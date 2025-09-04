#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>


#define THREAD_NUM 4
sem_t sem;

void* routine(void* args) {
    sem_wait(&sem);
    sleep(1);
    printf("Helo from thread %d\n", *(int*)args);
    sem_post(&sem);
    free(args);
}

int main(int argc, char *argv[]) {
    pthread_t th[THREAD_NUM];

    sem_init(&sem, 0, 3);

    int i;
    for (i=0; i<THREAD_NUM; ++i) {
        int *val = malloc(sizeof(int));
        *val = i;
        if (pthread_create(&th[i], NULL, &routine, val) != 0) {
            perror("Failed to create thread");
        }
    }

    for (i=0; i<THREAD_NUM; ++i) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }

    sem_destroy(&sem);

    return 0;
}