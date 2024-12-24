#include <stdio.h>
#include <pthread.h> /* POSIX threads */
#include <unistd.h>  /* For pause() and sleep() */


// pthreads have to return void pointer
void* myturn(void* arg) {
    while(1)
    {
        sleep(1);
        printf("My turn\n");
    }
    return NULL;
}

void yourturn() {
    while(1)
    {
        sleep(2);
        printf("Your turn\n");
    }
    // return NULL;
}

int main() {
    pthread_t newthread;

    pthread_create(&newthread, NULL, myturn, NULL);
    // pthread_create(&newthread, NULL, yourturn, NULL);
//     myturn();
    yourturn();
}