#include <stdio.h>

struct Person
{
    char name[64];
    int age;
};


int add(int a, int b) {
    return a+b;
}

int subtract(int a, int b) {
    return a-b;
}

// Function returning a pointer to an array of pointers to functions
int (*(*createFunctionPointerArray())[])() {
    static int (*functionPointers[])(int, int) = {&add, &subtract}; // Array of function Pointers
    return &functionPointers;
}

int main() {

    struct Person people[100];

    struct Person *p_person = &people;
    
    int i = 0;
    for (i=0; i<100; i++) {
        p_person->age = 0;
        // p_Person += sizeof(struct Person);
        p_person++;
    }


    // Pointer to an array of pointers to functions
    int (*(*functionPtrArray)[])() = createFunctionPointerArray();

    // Call first function in the arra and print the result
    int result = (*( (*functionPtrArray)[0]))(10, 5); // Call add(10, 5)
    printf("Result of add: %d\n", result);

    result = (*( (*functionPtrArray)[1]))(10, 5); 
    printf("Result of subtract: %d\n", result);

    return 0;
}