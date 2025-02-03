#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Define an enum for tagging the type
typedef enum {
    TAG_BOOL,
    TAG_INT,
    TAG_FLOAT,
    TAG_STRING,
    TAG_DOUBLE
} tag_t;

// Define a tagged union structure
typedef struct {
    tag_t tag;
    union {
        bool b;
        int i;
        float f;
        double d;
        char *s;  // String requires dynamic memory
    } value;
} dynamic_t;

// Function to safely assign values to the tagged union
void set_value(dynamic_t *dyn, tag_t type, void *data) {
    if (!dyn) return;

    // Free previous memory if needed
    if (dyn->tag == TAG_STRING && dyn->value.s) {
        free(dyn->value.s);
    }

    dyn->tag = type;
    switch (type) {
        case TAG_BOOL:
            dyn->value.b = *(bool *)data;
            break;
        case TAG_INT:
            dyn->value.i = *(int *)data;
            break;
        case TAG_FLOAT:
            dyn->value.f = *(float *)data;
            break;
        case TAG_DOUBLE:
            dyn->value.d = *(double *)data;
            break;
        case TAG_STRING:
            dyn->value.s = strdup((char *)data);  // Allocate memory for string
            break;
    }
}

// Function to print the stored value based on the tag
void print_value(const dynamic_t *dyn) {
    if (!dyn) return;

    printf("Stored value: ");
    switch (dyn->tag) {
        case TAG_BOOL:
            printf("%s\n", dyn->value.b ? "true" : "false");
            break;
        case TAG_INT:
            printf("%d\n", dyn->value.i);
            break;
        case TAG_FLOAT:
            printf("%.2f\n", dyn->value.f);
            break;
        case TAG_DOUBLE:
            printf("%.5lf\n", dyn->value.d);
            break;
        case TAG_STRING:
            printf("\"%s\"\n", dyn->value.s ? dyn->value.s : "(null)");
            break;
    }
}

// Function to clean up memory
void cleanup(dynamic_t *dyn) {
    if (dyn->tag == TAG_STRING && dyn->value.s) {
        free(dyn->value.s);
        dyn->value.s = NULL;
    }
}

int main() {
    dynamic_t dyn;

    bool b = true;
    set_value(&dyn, TAG_BOOL, &b);
    print_value(&dyn);

    int num = 42;
    set_value(&dyn, TAG_INT, &num);
    print_value(&dyn);

    float f = 3.14f;
    set_value(&dyn, TAG_FLOAT, &f);
    print_value(&dyn);

    double d = 12345.6789;
    set_value(&dyn, TAG_DOUBLE, &d);
    print_value(&dyn);

    char str[] = "Hello, Tagged Unions!";
    set_value(&dyn, TAG_STRING, str);
    print_value(&dyn);

    cleanup(&dyn);  // Free any allocated memory

    return 0;
}
