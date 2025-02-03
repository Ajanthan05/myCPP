#include <stdio.h>
#include <stdbool.h>

#define PREFIX(var_name) new_##var_name

#define GENERIC_AND_FUNC(type)          \
    type add_##type(type x, type y) {   \
        return x + y;                   \
    }

GENERIC_AND_FUNC(int)
GENERIC_AND_FUNC(float)
GENERIC_AND_FUNC(long)


#define CONCAT(a, b) a##b

/* Creating Struct Member Names Dynamically */
#define MEMBER(struct_name, member) struct_name##_##member

typedef struct {
    int person_age;
} Person;


#define PRINTF_LOOP(iterations, ...) do {   \
    for(int i=0; i<iterations; i++)        \
        printf(__VA_ARGS__);            \
} while(0)

int main() {

    int PREFIX(foo) = 123;
    printf("%d\n", new_foo);

    printf("%d\n", add_int(3, 5));
    printf("%f\n", add_float(3.5, 9.99));
    printf("%ld\n", add_long(1L,  2187699999998L));

    do { printf("%d\n", add_int(3, 5)); } while (0);

    // Useful for generating variable or function names dynamically.
    int CONCAT(my, Var) = 10;  // Expands to: int myVar = 10;

    Person p;
    p.MEMBER(person, age) = 25;  // Expands to: p.person_age = 25;
    printf("person_age: %d\n", p.person_age);


    PRINTF_LOOP(3, "hello %d %s\n", 32, "bar");

    return 0;
}

/*
Conditional Compilation Macros
Used for including or excluding code based on conditions.

#ifdef MACRO – Checks if MACRO is defined.
#ifndef MACRO – Checks if MACRO is not defined.
#if EXPRESSION – Checks if an expression evaluates to true.
#elif / #else – Else-if / Else for preprocessor conditions.
#undef MACRO – Undefines a macro.


#pragma pack(1)  // Aligns structure members to 1-byte boundaries
*/