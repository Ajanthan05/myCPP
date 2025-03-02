#include <stdio.h>

unsigned int clear_even_bits(unsigned int num) {
    return num & 0xAAAAAAAA; // Mask to retain odd bits and clear even bits
}

int main() {
    unsigned int num = 0xFFFFFFFF; // Example number (all bits set)
    unsigned int result = clear_even_bits(num);
    printf("Modified Number: 0x%X\n", result);
    return 0;
}
