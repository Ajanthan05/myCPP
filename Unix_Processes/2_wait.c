#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int id = fork();
    int n;
    if(id == 0) {
        n = 1;
    } else {
        n = 6;
    }

    if (id != 0) {
        wait(NULL);
    }

    int i;
    for(i=n; i<n+5; i++) {
        printf("%d ", i);
        fflush(stdout);
    }
    printf("\n");
    return 0;
}