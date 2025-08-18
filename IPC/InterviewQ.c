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
    PrintLL(head);
    // Swap(head, 4);

}

int main() {
    LL();
}