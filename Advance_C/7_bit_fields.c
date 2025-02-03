#include <stdio.h>
#include <stdbool.h>

struct foo
{
    int first_4 : 4;
    int next_4 : 4;
    int alone : 1;
    int rest : 7;
};

struct Flags {
    unsigned int is_enabled : 1;  // 1-bit flag (0 or 1)
    unsigned int mode : 2;        // 2-bit mode (0 to 3)
    unsigned int priority : 3;    // 3-bit priority (0 to 7)
};

int main() {

    printf("%zu\n", sizeof(struct foo));

    struct foo fl;
    fl.first_4 = 2;
    fl.next_4 = 4;

    struct Flags f = {1, 2, 5}; // Setting bit field values

    printf("is_enabled: %d\n", f.is_enabled);
    printf("mode: %d\n", f.mode);
    printf("priority: %d\n", f.priority);

    return 0;
}