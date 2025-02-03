#include <stdio.h>
#include <stdint.h>
// If u want char pointer which is modifiable
#include <stdlib.h>
#include <string.h>


/* OKBJDUMP 
.rodata : Read only data
*/


int main() {

    char *literal = "I am a string literal";
    char array[] = "I am a string array";
    array[0] = '#';
    // literal[0] = '#'; Segmentation fault
    /*
    objdump -s -j .rodata a.out 

    a.out:     file format elf64-x86-64

    Contents of section .rodata:
    2000 01000200 4920616d 20612073 7472696e  ....I am a strin
    2010 67206c69 74657261 6c00               g literal.     
 */

    printf("%s\n", literal);
    printf("%s\n", array);

    // If u want char pointer which is modifiable
    // strdub duplicate the string
    char *s = strdup(literal);  // stored in heap
    s[0] = '#';
    printf("%s\n", s);


    int a = 32;
    int hex = 0xffaa33;
    int octal = 0755;  // start with 0
    int binary = 0b1010101010;
    int64_t b = 966;

    printf("%d\n", a);
    printf("%x\n", hex);
    printf("%o\n", octal);
    printf("%b\n", binary);
    printf("%ld\n", b);
    // printf("%\n", );

    return 0;
}