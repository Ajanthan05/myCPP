#include <stdio.h>
#include "t.h"

// g++ main.c t.o here main need to be compile in c++ for
// that extern is used

int main() {

    int result = add(1, 2);
    printf("Result= %d\n", result);
    return 0;
}

/* 
g++ main.c t.o
/usr/bin/ld: /tmp/cc1Emj15.o: in function `main':
main.c:(.text+0x17): undefined reference to `add(int, int)'
collect2: error: ld returned 1 exit status


nm t.o
0000000000000000 T add
$ g++ -c t.c
$ nm t.o
0000000000000000 T _Z3addii

*/