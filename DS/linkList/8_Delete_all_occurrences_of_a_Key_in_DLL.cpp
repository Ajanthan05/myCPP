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

// vector<pair<int, int>> findPairs(Node* head, int k)
// {
//     Node *right = head, *left = head;
//     vector<pair<int, int>> ans;
//     while(right) {
//         right = right->next;
//     }

//     while(right > left) {
//         if (left->data + right->data == k) {
//             ans.push_back({left->data, right->data});
//             left = left->next;
//         }
//         else if (left->data + right->data > k) {
//             right = right->prev;
//         }
//         else {
//             left = left->next;
//         }
//     }
//     return ans;
// }
vector<pair<int, int>> findPairs(Node* head, int k) {
    vector<pair<int, int>> ans;
    if (!head) return ans;  // Handle empty list

    Node *left = head, *right = head;

    // Move right to the last node
    while (right->next) {
        right = right->next;
    }

    // Two-pointer approach
    while (left && right && left != right && left->prev != right) {
        int sum = left->data + right->data;

        if (sum == k) {
            ans.push_back({left->data, right->data});
            left = left->next;
            right = right->prev;
        } 
        else if (sum > k) {
            right = right->prev;
        } 
        else {
            left = left->next;
        }
    }
    return ans;
}

/* Remove duplicates from sorted DLL */
Node* deleteDuplicates(Node* head) {
    if (!head) return nullptr;  // Handle empty list case
    Node* tmp = head;

    while(tmp) {
        Node* newNode = tmp->next;
        while(newNode && newNode->data == tmp->data) {
            Node* duplicate = newNode;
            newNode = newNode->next;
            delete duplicate;
        }
        tmp->next = newNode;
        if (newNode) newNode->prev = tmp;

        tmp = newNode;
        // newNode = newNode->next;
    }
    return head;
}

int main() {


    return 0;
}