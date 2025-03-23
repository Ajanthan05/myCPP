#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#include <sys/stat.h>   // FIFO
#include <fcntl.h>      // open

int main() {
    // execlp("ping", "ping", "-c", "3", "google.com", NULL);
    // it replace the hole process
    // or it memory is going to replace by it's own memory
    
    int pid = fork();
    if (pid == -1) {
        printf("An error occure with fork\n");
        return 4;
    }
    
    if (pid == 0) {
        // Run it in child process
        execlp("ping", "ping", "-c", "3", "google.com", NULL);
        // how to check the launched p
    }
    else {
        wait(NULL);
        printf("Success!\n");
    }

    return 0;
}