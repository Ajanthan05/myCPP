#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#define ARRAY_LEN(a) (sizeof a / sizeof a[0])

void *my_memset(void *data, uint8_t c, size_t n) {
    uint8_t *data_as_bytes = (uint8_t*)data;
    for (size_t i=0; i<n; i++) {
        data_as_bytes[i] = c;
    }
    return data;
}

void map(void *xs, size_t n, size_t size, void (*f)(void*)) {
    uint8_t *xs_as_bytes = (uint8_t*)xs;
    for (size_t i=0; i<n; i++)
        f(&xs_as_bytes[i * size]);
}

void square(void *x) {
    int *p = x;
    *p = *p * *p;
}

int main() {
    int a = 5;
    void *p = &a;

    printf("%p\n", p);
    // printf("%p\n", *p); invalid use of void expression
    printf("%d\n", *(int*)p);

    printf("\n");
    int xs[5] = {1,2,3,4,5};
    for(int i=0; i<5; i++)
        printf("%d, ", xs[i]);
    printf("\n");

    // my_memset(xs, 3, sizeof xs);
    map(xs, 5, sizeof(int), square);

    for(int i=0; i<5; i++)
        printf("%d, ", xs[i]);
    printf("\n");

    // int x = 1_000_000; // Use underscore as a separator (C23 feature)
    // printf("%d\n", x); // -std=c2x

    int b = 1;
    int *b_ptr = &b;
    printf("%zu\n", sizeof b);
    printf("%zu\n", sizeof b_ptr);
    printf("%zu\n", ARRAY_LEN(xs));

    int *xs_ptr = xs;
    printf("%zu\n", sizeof xs_ptr);

    return 0;
}