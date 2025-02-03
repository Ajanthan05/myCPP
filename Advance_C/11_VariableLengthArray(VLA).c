#include <stdio.h>
#include <stdlib.h>
#include <alloca.h>

/* 
alloca() is very useful if you can't use a standard local variable because its size would need to be determined at runtime and you can absolutely guarantee that the pointer you get from alloca() will NEVER be used after this function returns.

You can be fairly safe if you

do not return the pointer, or anything that contains it.
do not store the pointer in any structure allocated on the heap
do not let any other thread use the pointer

✅ Benefits:
dynamic memory allocation at runtime without kernel calls
No kernel calls → Faster than malloc()
Automatic deallocation → No need to free()
Maintains program flow → Stack memory is managed automatically
❌ Limitations:
Limited by stack size (~8MB by default on Linux)
Function scope only → Memory is freed once function exits
Not standard C (but works in GCC, Clang, MSVC)

✅ Benefits of alloca()
1️⃣ Automatic Deallocation – Memory is freed automatically when the function returns (stack unwinding).
2️⃣ Faster than malloc() – No system call overhead; just adjusts the stack pointer.
3️⃣ No Need to Call free() – Simplifies memory management.

❌ Limitations of alloca()

Limited to Stack Size – Can cause stack overflow if large memory is allocated.
Scope Limited to Function – Memory is freed once the function exits.
Not Standard in C – Although widely available, it's not in the C standard (but supported in GCC, Clang, MSVC).

✅ Benefits of malloc()
1️⃣ More Flexible – Memory persists beyond the function scope.
2️⃣ Handles Large Allocations – Uses heap memory instead of stack.
3️⃣ Standard C Library Function – Works across all compilers.

❌ Limitations of malloc()

Slower due to system calls.
Must call free() to avoid memory leaks.


Advantages of alloca() function:

It provides faster allocation of the memory.
It uses very little extra space to allocate memory.
It does not cause memory fragmentation and space allocated to one block can be reused by another block of another size as it doesn’t contain any separate memory for different size blocks.
It automatically frees up the space when the function returns its value unlike malloc(), in which we have to remove the space allocated by the function.
Disadvantages of alloca() function:

Memory allocated by alloca() function is fixed and cannot be changed, and if you try to exceed that limit then you will get a segmentation fault for accessing memory that was not allocated.
Some system does not support alloca() function due to which it is not advisable to use this function instead of this, it is preferable to use the malloc() function in C programs.
It is dependent on the compiler as well as the machine.
*/
int main() {

    int length = 123;
    int *xs = (int*)alloca(length * sizeof(int));

    for(int i=0; i<length; i++) {
        xs[i] = i*i;
    }
    for(int i=0; i<length; i++) {
        printf("%d, ", xs[i]);
    }

    return 0;
}