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

Node* collisionPoint(Node *t1, Node *t2, int dis) {
    while(dis) {
        dis--;
        t2 = t2->next;
    }
    while (t1 != NULL) {
        if (t1 == t2) return t1;
        t1 = t1->next;
        t2 = t2->next; 
    }
    return nullptr;
}

Node* intersectPoint(Node *head1, Node *head2) {
    Node *temp1 = head1, *temp2 = head2;
    int l1 = 0, l2 = 0;

    while(temp1 != NULL) {
        temp1 = temp1->next;
        l1++;
    }
    while(temp2 != NULL) {
        temp2 = temp2->next;
        l2++;
    }

    if (l2 > l1) {
        return collisionPoint(head1, head2, l2-l1);
    }
    else {
        return collisionPoint(head2, head1, l1-l2);
    }
}

Node* intersectPoint_optimal(Node *head1, Node *head2) {
    Node *temp1 = head1, *temp2 = head2;

    while(temp1 !=  temp2) {
        temp1 = temp1->next;
        temp2 = temp2->next;

        if (temp1 == temp2) return temp1;
        if (temp1 == NULL && temp2 == NULL) return NULL;

        if (temp1 == NULL) temp1 = head2;
        if (temp2 == NULL) temp2 = head1;     
    }
    return temp1;
}

Node *getIntersectionNode(Node *l1, Node *l2) {
    if (l1 == NULL || l2 == NULL) return NULL;

    Node *a = l1;
    Node *b = l2;

    while (a != b) {
        if (a == NULL && b == NULL) return NULL;
        a = (a == NULL? l2 : a->next);
        b = (b == NULL? l1 : b->next);
    }
    return a;
}

int main() {
    // creation of first list: 10 -> 15 -> 30
    Node *head1 = new Node(10);
    head1->next = new Node(15);
    head1->next->next = new Node(30);

    // creation of second list: 3 -> 6 -> 9 -> 15 -> 30
    Node *head2 = new Node(3);
    head2->next = new Node(6);
    head2->next->next = head1->next;

    Node *intersectionPoint = intersectPoint(head1, head2);

    if (intersectionPoint == nullptr)
        cout << "-1";
    else
        cout << intersectionPoint->data << endl;

    
    Node *intersectionPoint_O = intersectPoint_optimal(head1, head2);

    if (intersectionPoint_O == nullptr)
        cout << "-1";
    else
        cout << intersectionPoint_O->data << endl;


    return 0;
}