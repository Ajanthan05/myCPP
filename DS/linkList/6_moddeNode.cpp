#include <iostream>
#include <vector>
#include <climits>

using namespace std;

struct Node {
public:
    int data;
    Node* next;


    Node() : data(0), next(nullptr) {}

    Node(int data) : data(data), next(nullptr) {}

    Node(int data, Node* next) : data(data), next(next) {}

};

    
Node* middleNode(Node* head) {
    Node* fast = head, *slow = head;

    while(fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}

/* To get the first middle node in an even-length linked list */

Node* firstMiddleNode(Node* head) {
    Node* fast = head, *slow = head;

    while (fast->next != NULL && fast->next->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}


int main() {


    return 0;
}