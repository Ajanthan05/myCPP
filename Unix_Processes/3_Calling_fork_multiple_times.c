#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#include <sys/wait.h>
#include <errno.h>

/*
_________________
|               |  It has 2 children wait function wait for only one.
|   id1 = X     |====================
|   id2 = Z     |                   |
|_______________|                   |
                                    |
                                    |
                                    |
                            _________________
                            |  z            |
                            |   id1 = X     |
                            |   id2 = 0     |
                            |_______________|                              
_________________
|  x            |           
|   id1 = 0     |
|   id2 = Y     |
|_______________|


_________________
|  y            |
|   id1 = 0     |
|   id2 = 0     |
|_______________|

*/

int main() {
    int id1 = fork();
    int id2 = fork();

    if(id1 == 0) {
        if (id2 == 0) {
            printf("We are process y\n");
        } else {
            printf("We are process x\n");
        }
    } else {
        if (id2 == 0) {
            printf("We are process z\n");
        } else {
            printf("We are the parent process\n");
        }
    }
    // wait(NULL) return -1 when there is nothing to wait for
    while(wait(NULL) != -1 || errno != ECHILD) {
        printf("Waited for child to finish\n");
    }

    return 0;
}