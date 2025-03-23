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
Those file descriptors are also assigned to teh newly created process so they get inherited
If u close the file descripter at the one point in a process , the other process 
remain open
*/
int main() {
    int arr[] = {1,2,3,4,5,6};
    int mid = sizeof(arr)/sizeof(int)/2;
    int s, e;

    int fd[2];
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
        s = 0;
        e = mid;
    }
    else {
        s = mid;
        e = 2*mid;
    }

    int sum = 0;
    for(int i=s; i<e; i++) {
        sum += arr[i];
    }
    
    printf("Calukulate Partial Sum: %d\n", sum);

    if (id == 0) {
        close(fd[0]);
        if (write(fd[1], &sum, sizeof(sum)) == -1) {
            printf("Error while writing to the pipe\n");
            return 2;
        }
        close(fd[1]); // after close the write end reader can read
    }
    else {
        close(fd[1]);
        int sumFromeChild;

        if (read(fd[0], &sumFromeChild, sizeof(int)) == -1) {
            printf("Error while reading from the pipe\n");
            return 2;
        }
        close(fd[0]);
        // printf("Value: %d\n", sumFromeChild);

        wait(NULL);
        int totalSum = sum + sumFromeChild;
        printf("Total Sum: %d\n", totalSum);

    }
    return 0;
}