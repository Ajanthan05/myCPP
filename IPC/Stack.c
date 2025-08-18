#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h> 

#define STACK_EMPTY INT_MIN

typedef struct Node {
    int value;
    struct Node *next;
} Node; 

typedef Node* Stack;

bool push(Stack *head, int val) {
    Node *newNode = malloc(sizeof(Node));
    if (!newNode) return false;
    
    newNode->value = val;
    newNode->next = *head;
    *head = newNode;
    return true;
}
int pop(Stack *head) {  // Node** head  // A pointer to a pointer to Node
    if (!head || !*head) return STACK_EMPTY;

    int res = (*head)->value;
    Node *tmp = *head;
    *head = (*head)->next;
    free(tmp);
    return res;
}
void LL() {
    Stack s1=NULL, s2=NULL;
    push(&s1, 1);
    push(&s1, 2);
    push(&s1, 3);

    int t;
    while((t = pop(&s1)) != STACK_EMPTY) {
        printf("%d, ", t);
    }

    printf("\n");
}

#define MAX 10

typedef struct StachArr {
    int val[MAX];
    int top;
} StachArr;

bool pushA(StachArr *st, int value) {
    if ((*st).top >= MAX) return false;

    (*st).top++;
    (*st).val[(*st).top] = value;

    return true;
}
int pop(StachArr *st) {
    int res = (*st).top;
}
void TestStakcArr() {
    StachArr s;
    pushA(&s, 5);
}
int main() {
    LL();
    TestStakcArr();
    return 0;
}
/*
void push(struct Node *head, int val) {
    // head is a COPY of s1
    // changes here don't affect s1 in main()
}
❌ s1 remains unchanged in main().*/



