#include <stdio.h>
#include <pthread.h> /* POSIX threads */
#include <unistd.h>  /* For pause() and sleep() */
#include <stdlib.h>  /* exit() */

static void *thread_fn_callback(void *arg) {
    char *input = (char *)arg;

    while(1) {
        printf("Input string = %s\n", input);
        sleep(1);
    }
}

void thread1_create() {
    pthread_t pthread1;  // This is called thread handle

    // Always pass persistance memory as an argument to the thread
    // Dont pass caller's local variable or stack memory

    static char *thread_input1 = "I am thread no 1";

    int rc = pthread_create(&pthread1,   // thread handle
                    NULL,
                    thread_fn_callback,  // pointer to the function
                    (void *)thread_input1);  // memory to the thread 
                                            // it should be memory on the heap or static memory
                                            // never pass address of local variable or address 
                                            // of memory in the stack
    // boc while the new thread start executing "thread1_create" going to be return. Means it will complete it execution
    // memory pass chould not be in stack or local variable
    if(rc != 0) {
        printf("Thread could not be created, errno = %d\n", rc);
        exit(0);
    }
}

int main(int argc, char **argv) {

    thread1_create();
    printf("main fn paused\n");
    // pause();
    pthread_exit(0);
    printf("main fn paused\n");
    return 0;
}


/*
Thread Termination
1. threan fn return
2. pthread_exit(0);
3. thread Cancellation

If main thread dies all other threads dies by default, but vice-versa is not true
All threads are siblings

Thread of the same process among themself share the Heap Memory, Sockets, File Descriptors, Global Variables
  Resource allocated by one threas is visible to rest of the others

What threads do not share is the stack memory, every thread has its own stack memory

Stach memory 
    used to inorder to support processor call and return

NO separate virtual memory for newly created thread

All thread share the same VAS (Virtual Address Space)

Rest of the VAS are shared by all threads.

Kernel (OS) do not schedule process, it schedules threads.

Thread is aschedulable entity, not a process

if a thread seg-fault, entire process is terminated (including all threads)

A signal is deliverde per process,vnot per thread
eg of signal is segmentation-fault signal

when program running ctrl+c will terminate entire process
ctrl+c give sigterm signal
*/