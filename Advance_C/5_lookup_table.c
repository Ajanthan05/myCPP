#include <stdio.h>
#include <stdbool.h>

enum foo {
    foo_2 = 0,
    foo_3,
    foo_4,
};

static int squares[] = {
    4,
    9,
    16
};

// But new
static int squaresN[] = {
    [foo_4] = 4,
    [foo_2] = 9,
    [foo_3] = 16
};

#define FLAG_FOO 1 << 10

static char case_convert[] = {
    ['b'] = 'B',
    ['a'] = 'A',
    ['c'] = 'D',
    // [FLAG_FOO] = 'Q'
};

int main() {
    printf("%d\n", squares[foo_2]);
    printf("%d\n", squares[foo_3]);
    printf("%d\n", squares[foo_4]);

    printf("\nNew\n");
    printf("%d\n", squaresN[foo_2]);
    printf("%d\n", squaresN[foo_3]);
    printf("%d\n", squaresN[foo_4]);

    printf("\nChar\n");
    printf("%c\n", case_convert['c']);

    printf("%zu\n", sizeof case_convert); // 100  CURRENT SIZE 1025

    return 0;
}