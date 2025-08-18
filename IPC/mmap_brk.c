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

brk move the program brake. (Expanding or string the heap)

int brk(void *addr);

void *sbrk(intptr_t increment);  // sbrk is just a raper around brk

The program break is the first location after the end of uninitialized data segment



sbrk: return address of previous break
page size is 4K bottom 12 bits are always going to be zero

mmap request memory from kernel


Low Address
+---------------------+ ← Start of .text
| Text Segment (.text)|
+---------------------+
| Initialized Data    | (.data)
+---------------------+
| Uninitialized Data  | (.bss)
+---------------------+
| Heap Segment        | ← malloc() grows this up
|                     |
| Program Break →     | ← End of current heap
+---------------------+
|                     |
| (Unmapped Gap)      |
|                     |
| Stack Frame         | ← Grows downward
+---------------------+
High Address


.text: Code (read-only)
.data: Global/static variables with values
.bss: Global/static variables uninitialized
Program break: End of .bss and .data, start of the heap

Heap: Used by malloc, calloc, etc.

Stack: Function calls, local variables

Uninitialized static variables → .bss
Initialized static variables → .data



.bss (Block Started by Symbol) Segment (Uninitialized Data Segment)?
🔹 .bss stands for Block Started by Symbol
It's a section in the memory layout of a C program that contains uninitialized global 
and static variables.

❌ Not exactly. “Stack allocation happens at compile time.”
Stack layout and variable sizes are determined at compile time.
Actual memory allocation for local variables happens at runtime, when the function 
is called.
🔷 Detailed Breakdown:
🔹 🔧 Compile Time:
The compiler knows:

How much stack space the function needs (e.g., for 3 int variables, it might reserve 12 bytes).

Where each local variable will be within the stack frame (offsets).

But the memory is not actually reserved yet.

🔹 ⏱️ Runtime:
When the function is called, the stack pointer (SP) is adjusted to allocate space.
A stack frame is created:
    1. Stores return address
    2. Stores function parameters
    3. Stores local variables
When the function returns:
    Stack frame is popped (deallocated automatically).



gcc main.c -o myapp
./myapp &
pmap $(pidof myapp)
You'll see output like:

00400000    132K r-x-- myapp             # .text
00621000      4K r---- myapp
00622000      4K rw--- myapp             # .data/.bss
00e00000     64K rw--- [heap]            # Heap
7f1f4b7c7000 1.2M r-x-- libc.so.6        # Shared library (libc)
7fff25eae000 132K rw--- [stack]          # Stack

You can inspect process layout more deeply using:
    pmap, cat /proc/<pid>/maps
    ldd ./your_binary (to see linked shared libraries)
    valgrind or gdb for heap and memory analysis
*/

int myfunction(int counter) {       
    int result = 5;
    printf("myfunction %p %p %p\n", &result, &counter, __builtin_frame_address(0));
    /*  
    To print the result in GDM
    (GDB): x/128xb (give frame address)
    */
    return 0;
}

/*
Text Segment (Code Segment):
Stores the compiled machine code of the program, including functions and constant data. This segment is usually read-only.
Data Segment:
Stores initialized global and static variables. These variables have a fixed size and are allocated memory before the program starts execution.
BSS (Block Started by Symbol) Segment:
Stores uninitialized global and static variables. These are automatically initialized to zero by the system.
Heap Segment:
Used for dynamic memory allocation. Memory is allocated and deallocated during runtime using functions like malloc(), calloc(), realloc(), and free(). This allows for flexible memory management when the exact memory requirements are not known at compile time.
Stack Segment:
Used for automatic memory allocation. It stores local variables, function parameters, and return addresses for function calls. Memory on the stack is allocated and deallocated automatically as functions are called and return.


Static/Automatic Memory Allocation:
Memory is allocated at compile time or when a function is called (for local variables).
Size is fixed and determined at compile time.
Managed automatically by the system (e.g., stack for local variables, data/BSS for global/static variables).
Dynamic Memory Allocation:
Memory is allocated at runtime from the heap.
Size can be determined and adjusted during program execution.
Requires explicit management by the programmer using functions like malloc(), calloc(), realloc(), and free(). Failure to free() allocated memory can lead to memory leaks.

*/
// #define PAGESIZE 4096

int main() {
    int i = 0;
    printf("main: %p %p\n", &i, __builtin_frame_address(0));
    myfunction();

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
    uint8_t* First = (uint8_t*)mmap(NULL, PAGESIZE, PROT_READ | PROT_WRITE,
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