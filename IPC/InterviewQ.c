#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h> 

#define STACK_EMPTY INT_MIN

typedef struct Node {
    int value;
    struct Node *next;
} Node; 

Node* createNode(int val) {
    Node *head = malloc(sizeof(Node));
    if (!head) return NULL;
    head->value = val;
    return head;
}

void Swap(Node *head, int ind) {
    ind--;
    Node * tmp = head;
    while(ind--, !tmp) tmp = tmp->next;

    Node *s1 = tmp->next;
    Node *s2 = tmp->next->next;

    
    tmp->next = s2;
    s2->next = s1;
}
void PrintLL(Node *head) {
    while(head) {
        printf("%d ", head->value);
        head = head->next;
    }
    printf("\n");   
}
void LL() {
    Node *head = createNode(1);
    Node * tmp = head;
    
    for(int i=2; i<=5; i++) {
        tmp->next = createNode(i);
        tmp = tmp->next;
    }
    if (!tmp || tmp->next)
    PrintLL(head);
    // Swap(head, 4);

}

// Uninitialized pointers are known as wild pointers because they point to some arbitrary memory location and may cause a program to crash or behave unexpectedly.
// C program that demonstrated wild pointers
int WildPtr()
{
    /* wild pointer */
    int* p;
    /* Some unknown memory location is being corrupted.
    This should never be done. */
    *p = 12;

    // If we want a pointer to a value (or set of values) without having a variable for the value, we should explicitly allocate memory and put the value in the allocated memory.
    int *ptr = (int *)malloc(sizeof(int));
    *ptr = 15;
}

int main() {
    LL();
}