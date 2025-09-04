#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#include <stdarg.h>

int sum(int count, ...) {
    va_list args;
    va_start(args, count);

    int sum=0;
    for(int i=0; i<count; ++i) {
        int x = va_arg(args, int);
        sum += x;
    }
    va_end(args);
    return sum;
}

// Null or 0 will terminate
int sum2(int a, ...) {
    va_list args;
    va_start(args, a);

    int sum=a;
    int x = va_arg(args, int);

    while(x!=0) {    
        sum += x;
        x = va_arg(args, int);
    }
    va_end(args);
    return sum;
}

void add_to_array(int* arr, int* last_ind, int elem) {
    arr[*last_ind] = elem;
    (*last_ind)++;
}
void Array() {
    int* arr = malloc(sizeof(int) * 4);
    int last_ind = 0;

    add_to_array(arr, &last_ind, 1);
    add_to_array(arr, &last_ind, 2);
    add_to_array(arr, &last_ind, 3);
    add_to_array(arr, &last_ind, 4);

    int* realloc_result = realloc(arr, sizeof(int) * 8);
    if(realloc_result==NULL) {
        free(arr);
        perror("Realloc Failed for arr");
        return;
    }
    arr = realloc_result;

    arr[last_ind] = 5;
    last_ind++;
}


void add_to_array2(int** arr, int* last_ind, int* capacity, int elem) {
    if (*last_ind >= *capacity) {
        *capacity *= 2;
        int* realloc_result = realloc(*arr, sizeof(int) * (*capacity));
        if(realloc_result==NULL) {
            free(arr);
            perror("Realloc Failed for arr");
            return;
        }
        *arr = realloc_result;
    }
    (*arr)[*last_ind] = elem;
    (*last_ind)++;
}
void Print(int* arr, int size) {
    for (int i=0; i<size; i++) {
        printf("%d ", (arr[i]));
    }
    printf("\n");
}

void Array2() {
    int capacity = 4;
    int* arr = malloc(sizeof(int) * capacity);
    int last_ind = 0;

    add_to_array2(&arr, &last_ind, &capacity, 1);
    add_to_array2(&arr, &last_ind, &capacity, 2);
    add_to_array2(&arr, &last_ind, &capacity, 3);
    add_to_array2(&arr, &last_ind, &capacity, 4);
    add_to_array2(&arr, &last_ind, &capacity, 5);

    Print(arr, last_ind);
}

void Array2D() {
    int rows = 3, cols = 4;

    int **arr = (int **)malloc(sizeof(int *) * rows);
    if (arr==NULL) {
        printf("Memory allocation failed");
        return;
    }

    for(int i=0; i<rows; ++i) {
        arr[i] = (int *)malloc(sizeof(int) * cols);
        if (arr[i] == NULL) {
            printf("Memory allocation failed\n");
            return;
        }
        for(int j=0; j<cols; ++j) {
            arr[i][j] = i * cols + j;
        }
    }    

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%2d ", arr[i][j]);
        }
        printf("\n");
    }

    for(int i=0; i<rows; ++i) {
        free(arr[i]);
    }
    free(arr);
}

void reverse_string(char *str) {
    int left=0;
    int right = strlen(str) - 1;
    char temp;

    while (left < right)    {
        temp = str[left];
        str[left] = str[right];
        str[right] = temp;

        left++;
        right--;
    }
}
void T_reverse_string() {
    char str[100];
    fgets(str, sizeof(str), stdin);

    // str[strcspn(str, "\n")] = '\0';

    reverse_string(str);

    printf("Reverse: %s\n", str);
}

int main() {
    Array2();
    Array2D();
    T_reverse_string();
}