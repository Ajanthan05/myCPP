#include <stdio.h>
#include <stdbool.h>  // Required for 'bool' type

enum tag
{
    TAG_BOOL,
    TAG_INT,
    TAG_FLOAT
};

struct dynamic
{
    enum tag tag;
    union {
        bool b;
        int i;
        float f;
    } value;
};

int main() {
    struct dynamic d;
    printf("%zu\n", sizeof d);

    d.tag = TAG_FLOAT;
    d.value.f = 42.55;

    switch (d.tag)
    {
        case TAG_BOOL: printf("bool : %d\n", d.value.b); break;
        case TAG_INT: printf("int : %d\n", d.value.i); break;
        case TAG_FLOAT: printf("float : %f\n", d.value.f); break;
    }

    return 0;
}