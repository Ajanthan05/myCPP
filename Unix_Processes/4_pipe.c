#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

// Pipe is in memory file
/* Aside from just getting copied over like 
only the self from those integrals getting copued over behind the scenes
Those file descriptors are also assigned to the newly created process so they get 
inherited If u close the file descripter at the one point in a process , the other 
process  remain open
*/
int main() {
    int fd[2];
    // fd[0] - read
    // fd[1] - write

    if (pipe(fd) == -1) {
        printf("An error occure while opening the pipe\n");
        return 1;
    }
    int id = fork();
    if (id == -1) {
        printf("An error occure with fork\n");
        return 4;
    }
    if (id == 0) {
        close(fd[0]);
        int x;
        scanf("%d", &x);

        if (write(fd[1], &x, sizeof(int)) == -1) {
            printf("Error while writing to the pipe\n");
            return 2;
        }
        close(fd[1]); // after close the write end reader can read
    }
    else {
        close(fd[1]);
        int y;

        if (read(fd[0], &y, sizeof(int)) == -1) {
            printf("Error while reading from the pipe\n");
            return 2;
        }
        close(fd[0]);
        printf("Value: %d\n", y);
    }
    
    return 0;
}