#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

#include <stdlib.h>
#include <sys/mman.h>  
#include <unistd.h>    

#include <string.h>
#include "Shared_Memory.h"

int main(int argc, char *argv[]) {
    if (argc != 1) {
        printf("Usage - %s // no args", argv[0]);
        return -1;
    }

    
}