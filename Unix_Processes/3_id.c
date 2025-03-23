#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

int main() {
    int id = fork();

    if(id == 0) {
        sleep(1);
    }

    printf("Process ID: %d, Current ID: %d, Parent ID: %d\n", id, getpid(), getppid());

    int res = wait(NULL);
    if (res == -1) {
        printf("No child to wait for\n");
    }
    else {
        printf("%d finished execution\n", res);
    }

    // Wait return -1 when there is nothing to wait for
    while(wait(NULL) != -1 || errno != ECHILD) {
        printf("Waited for achild to finish\n");
    }
    
    return 0;
}