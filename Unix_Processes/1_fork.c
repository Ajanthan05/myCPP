#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int nain() {
    int id = fork();
    printf("Process ID: %d\n", id);

    if(id == 0) {
        printf("Child Process\n");
    } else {
        printf("Main process");
    }
}