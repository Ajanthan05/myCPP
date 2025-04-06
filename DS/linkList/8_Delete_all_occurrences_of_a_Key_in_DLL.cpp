#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>

using namespace std;

// class Node {
// private:
//     int data;
//     Node* prev;
//     Node* next;

//     Node() : data(0), prev(nullptr), next(nullptr) {}

//     Node(int data) : data(data), prev(nullptr), next(nullptr) {}

//     Node(int data, Node* next, Node* prev) : data(data), prev(prev), next(next) {}
// };

/* Remove Private Access Modifier: Since prev, next, and data are typically accessed externally in linked lists, making them public avoids unnecessary getters/setters.

Use explicit for Single Argument Constructor: Prevents implicit conversions.

Consolidate Constructors: One constructor with default arguments eliminates redundancy.

Avoid Unnecessary Copies: Pass pointers directly without copying.
*/

class Node {
public:
    int data;
    Node* prev;
    Node* next;

    explicit Node(int data = 0, Node* next = nullptr, Node* prev = nullptr) 
        : data(data), prev(prev), next(next) {}
};

Node * deleteAllOccurrences(Node* head, int k) {
    Node* tmp = head;
    while(tmp) {
        if (tmp->data == k) {
            if (tmp == head) {
                head = head->next;
            }
            Node* nextNode = tmp->next;
            Node* prevNode = tmp->prev;

            if (!nextNode) nextNode->prev = prevNode;
            if (!prevNode) prevNode->next = nextNode;
            
            delete tmp;
            tmp = nextNode;
        }
        else {
            tmp = tmp->next;
        }
    }
    return head;
}

/* Optimal */
Node* deleteAllOccurrences(Node* head, int k) {
    Node* tmp = head;
    
    while (tmp) {
        if (tmp->data == k) {
            Node* nextNode = tmp->next;
            Node* prevNode = tmp->prev;

            if (prevNode) prevNode->next = nextNode;
            else head = nextNode;  // Update head if deleting the first node

            if (nextNode) nextNode->prev = prevNode;

            delete tmp;
            tmp = nextNode; // Move to the next node
        } else {
            tmp = tmp->next; // Move forward if no deletion
        }
    }
    return head;
}

/* Find all Pairs with given Sum in DLL */
vector<pair<int, int>> findPairs(Node* head, int k)
{
    Node* tmp1 = head;
    vector<pair<int, int>> ans;
    while(tmp1) {
        Node* tmp2 = tmp1->next;
        while(tmp2 && tmp1->data + tmp2->data <= k) {
            if (tmp1->data + tmp2->data == k) {
                ans.push_back({tmp1->data, tmp2->data});
            }
            // else if(tmp1->data + tmp2->data > k) break;

            tmp2 = tmp2->next;
        }
        tmp1 = tmp1->next;
    }
    return ans;
}


int main() {


    return 0;
}