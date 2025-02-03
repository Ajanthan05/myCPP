#include <stdio.h>
#include <stdbool.h>

static const size_t len = 10; // declared in static memory and cannot modify
// U cannot use len to declare a static array

// static int xs[len] = {1};  // Lenth of static array has to known at compile time
/* The error "variably modified ‘xs’ at file scope" occurs because len is not a compile-time constant in the way the compiler expects when declaring an array at file scope.

In C, const variables have internal linkage, meaning their value is determined at runtime and stored in memory. This prevents them from being used for static array sizes at file scope.

Use #define or constexpr (C++)
#define LEN 10   // ✅ Compile-time constant
constexpr size_t len = 10;  // ✅ Compile-time constant in C++
enum { len = 10 };  // ✅ Compile-time constant in C
*/

constexpr size_t len2 = 10;
static int xs[len2] = {1};

struct foo {
    int a;
    int b;
};

constexpr struct foo f = {.a = 1, .b = 2};

staric struct foo f = {.a = 1, .b = 2}; // staric memory of executable

int main() {

    printf("\n");
    return 0;
}