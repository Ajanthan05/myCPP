#include <stdio.h>
#include <stdarg.h>

int sum_all(int x, int count, ...)
{
    va_list ap;
    va_start(ap, count);

    int sum = 0;

    for(int i=0; i<count; i++) {
        int x = va_arg(ap, int);
        sum += x;
    }
    va_end(ap);
    printf("x: %d\n", x);

    return sum;
}



int main() {

    printf("%d\n", sum_all(3,2,6,7));
    return 0;
}