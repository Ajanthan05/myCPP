#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

#include <stdlib.h>
#include <sys/mman.h>   // <-- Required for mmap, PROT_*, MAP_*
#include <unistd.h>     // <-- Required for getpagesize()
/*
    Stack
    --------------------


    --------------------
    library
    --------------------



    --------------------
    Program break
    --------------------
    Heap
    Globals
    Code
    --------------------



*/

/*  Program break present on top of heap (It called Bringer of Seg faults)

sbrk: return address of previous break
page size is 4K bottom 12 bits are always going to be zero

mmap request memory from kernel
*/
// #define PAGESIZE 4096

int main() {
    void *first  = sbrk(0);
    void *second = sbrk(4096);
    void *third  = sbrk(0);

    // Try to store something in the memory after the program break
    // int *ptr = (int *)third+1;
    // *ptr = 5;

    printf("first : %p\n", first);
    printf("second: %p\n", second);
    printf("third : %p\n", third);


    // mmap
    size_t PAGESIZE = getpagesize();  // or just #define PAGESIZE 4096
    uint8_t* First = mmap(NULL, PAGESIZE, PROT_READ | PROT_WRITE,
                          MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    /*  NULL (Write it anywhere), PAGESIZE, PROT_READ | PROT_WRITE,
                          MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
                          
    MAP_PRIVATE | MAP_ANONYMOUS  tell the kernel how we want the memory to be managed
    like want it to be private to a particular process 
    
    -1, 0  last 2 are useful for memory mapped files 


    A page is the smallest fixed-length contiguous block of virtual memory that the 
operating system manages via paging. It acts as a unit of data for memory management, 
memory protection, and virtual memory translation.

🔹 Typical Page Size
Most systems use 4 KB (4096 bytes) as the standard page size:

4 KB = 2¹² bytes → 12 bits
So, the bottom 12 bits of any page-aligned address are zero.

Address: 0x00403000
In binary: 0000 0000 0100 0000 0011 0000 0000 0000
The bottom 12 bits (0000 0000 0000) are zero — this is page-aligned.
Any address that starts a page (allocated by mmap, brk, or the kernel) is aligned this way.
    

*/
    if (First == MAP_FAILED) {
        perror("mmap");
        return EXIT_FAILURE;
    }

    // Example usage
    First[0] = 42;
    printf("Value at First[0]: %d\n", First[0]);

    munmap(First, PAGESIZE);
    return 0;
}