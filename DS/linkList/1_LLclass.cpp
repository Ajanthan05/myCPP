#include <iostream>
using namespace std;

class ListNode {
public:
    int val;
    ListNode *next;

    // Default constructor
    ListNode() : val(0), next(nullptr) {}

    // Constructor with a value
    ListNode(int x) : val(x), next(nullptr) {}

    // Constructor with a value and next pointer
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

// Function to print the list
void printList(ListNode* head) {
    while (head != nullptr) {
        cout << head->val << " -> ";
        head = head->next;
    }
    cout << "nullptr" << endl;
}

// Function to add a node at the end
ListNode* addNode(ListNode* head, int value) {
    ListNode* newNode = new ListNode(value);
    if (head == nullptr) {
        return newNode;
    }
    ListNode* temp = head;
    while (temp->next != nullptr) {
        temp = temp->next;
    }
    temp->next = newNode;
    return head;
}

// Function to delete a node by value
ListNode* deleteNode(ListNode* head, int value) {
    if (head == nullptr) return nullptr;

    // If the node to delete is the head
    if (head->val == value) {
        ListNode* temp = head->next;
        delete head;
        return temp;
    }

    ListNode* temp = head;
    while (temp->next != nullptr && temp->next->val != value) {
        temp = temp->next;
    }

    if (temp->next != nullptr) {
        ListNode* toDelete = temp->next;
        temp->next = temp->next->next;
        delete toDelete;
    }

    return head;
}

int main() {
    // Create an empty list
    ListNode* head = nullptr;

    // Add some nodes
    head = addNode(head, 1);
    head = addNode(head, 2);
    head = addNode(head, 3);
    head = addNode(head, 4);

    cout << "Original List: ";
    printList(head);

    // Delete a node
    head = deleteNode(head, 2);
    cout << "After Deleting 2: ";
    printList(head);

    // Add a new node
    head = addNode(head, 5);
    cout << "After Adding 5: ";
    printList(head);

    // Clean up the memory
    while (head != nullptr) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }

    return 0;
}
