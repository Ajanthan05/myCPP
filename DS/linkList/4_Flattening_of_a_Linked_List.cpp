#include <iostream>
#include <vector>
#include <climits>


// Definition for a Node.
class Node {
public:
    int val;
    Node* prev;
    Node* next;
    Node* child;
};

class Solution {
public:
    Node* mergeTwoLL(Node* a, Node* b) {
        Node* temp = new Node();
        Node *res = temp;

        while(a != NULL && b != NULL) {
            if (a->val < b->val) {
                temp->child = a;
                temp = temp->child;
                a = a->child;
            }
            else {
                temp->child = b;
                temp = temp->child;
                b = b->child;
            }
        }

        // then take all the elements
        if(a != NULL) temp->child = a;
        else temp->child = b;
        return res->child;
    }

    Node* flatten(Node* head) {
        if (head == NULL || head->next == NULL) return head;

        head->next = flatten(head->next);

        head = mergeTwoLL(head, head->next);

        return head;
    }
};
int main() {


    return 0;
}