#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

#include <stdlib.h>
#include <sys/mman.h>   // <-- Required for mmap, PROT_*, MAP_*
#include <unistd.h>     // <-- Required for getpagesize()

/*  Fork clone the address space
tfok
shmget:- use the key to create or get a block of shared memory associated with that key. It going to return shared memory ID
shmat:- It use block ID to map the block into this process address space and give a pointer to that block
shmat
shmdt
shmctl

"a.c" -> ftok -> "key" -> shmget -> "block" -> shmat -> "ptr" -> 
*/

int main() {
    int v = 5;

    size_t PAGESIZE = getpagesize();
    uint8_t *shared_memory = mmap(NULL, PAGESIZE,
                                    PROT_READ | PROT_WRITE,
                                    MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *shared_memory = 5;                          

    if (fork() == 0) { // Child
        v = 80;
        *shared_memory = 15;
    }
    else {
        wait(NULL);  // Parent wait for child
    }

    printf("Not Shared: %i\n", v);
    printf("shared_memory: %i\n", *shared_memory);
}