#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <stdbool.h>

#include <semaphore.h>
#include <fcntl.h>    // for O_CREAT, O_EXCL
#include <sys/stat.h> // for mode constants


// Attach a shared memory block
// Associated with filename
// Create it if it doesn't exist
char *attach_memory_block(char *filename, int size);
bool detach_memory_block(char *block); 
bool destroy_memory_block(char *filename); 

#define BLOCK_SIZE 4096
#define FILENAME "writeshmem.c"
#define IPC_RESULT_ERROR (-1)

// Filenames for 2 semaphores
#define SEM_PRODUCER_FNAME "/myproducer"
#define SEM_CONSUMER_FNAME "/myconsumer"

/* Value returned if `sem_open' failed.  */
// #define SEM_FAILED      ((sem_t *) 0)

#endif