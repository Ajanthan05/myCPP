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
    if (mkfifo("myfifo1", 0777) == -1) {
        if (errno != EEXIST) {
            printf("Could not create fifo file\n");
            return 1;
        }
    }

    printf("Opening...\n");
    // cat myfifo1
    // It also wait for write end to be opened
    int fd = open("myfifo1", O_WRONLY);  // O_RDWR (READ AND WRITE) it doesn't wait
    if (fd == -1) return 3;
    
    printf("Opened\n");
    int x = 'A';
    if (write(fd, &x, sizeof(x)) == -1) return 2;
    close(fd);

    return 0;
}