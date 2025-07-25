#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void copy_string(char *dest, const char *src) {
    while ((*dest++ = *src++) != '\0');
}
void my_free(char *src) {
    while(*src != '\0') {
        free(src);
    }
}
 
int main() {
    char *str1 = "Hello";
    char *str2;
 
    str2 = (char *)calloc(strlen(str1) + 1, sizeof(char));
    if (str2 == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
 
    copy_string(str2, str1);
 
    printf("Copied string: %s\n", str2);
    free(str2);
    return 0;
}