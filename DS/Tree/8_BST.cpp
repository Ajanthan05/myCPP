#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>

using namespace std;

struct Node {
    int val;
    Node *left;
    Node *right;

    explicit Node(int val) : val(val), left(nullptr), right(nullptr) {}
};

Node *searchBST(Node *root, int data) {
    while(root != NULL && root->val != data) {
        root = (data < root->val) ? root->left : root->right;
    }
    return root;
}

/* CEIL:- smallest val grater than or equal to input*/
int findCeil(Node* root, int input) {
    if (root == NULL) return -1;

    int ceil = -1;

    while(root) {
        if (root->val == input) return root->val;
        else if (root->val < input) root = root->right;
        else {
            ceil = root->val;
            root = root->left;
        }
    }
    return ceil;
}

/* Floor in a Binary Search Tree 
Gratest val smaller than or equal to val */
// int findFloor(Node* root, int input) {
//     if (root == NULL) return -1;

//     int floor = INT_MAX;

//     while(root) {
//         if (root->val == input) return root->val;
//         else if (root->val > input) root = root->left;
//         else {
//             // ceil = root->val;
//             root = root->left;
//         }
//     }
// }

/* Insert a given Node in Binary Search Tree */
Node *insertIntoBST(Node *root, int val) {
    if (!root) return new Node(val);
    Node *cur = root;
    while(true) {
        if (cur->val <= val) {
            if (cur->right) cur = cur->right;
            else {
                cur->right = new Node(val);
                break;
            }
        }
        else {
            if (cur->left != NULL) cur = cur->left;
            else {
                cur->left = new Node(val);
                break;
            }
        }
    }
    return root;
}

void Delete(Node* root, int val) {
    if (!root) return;

    Node *find = searchBST(root, val);
    if (find != NULL) return;
}

/* L45. K-th Smallest/Largest Element in BST 
Use inorder will always be in sorted order
Use counter to avoid space complexity
*/

int kthSmallest(Node* root, int k) {
    if(!root) return NULL;
    int cnt = 0;
    int ans = INT_MAX;
    while(k) {
        if (root->left) {
            root = root->left;
            ans = root->val;
        }
        else {
            root = root->right;
            ans = root->val;
        }
        k--;
    }
}

int main() {

    Node* head = new Node(6);
    insertIntoBST(head, 3);
    insertIntoBST(head, 8);
    insertIntoBST(head, 7);
    insertIntoBST(head, 2);
    insertIntoBST(head, 5);
    insertIntoBST(head, 4);
    // insertIntoBST(head, );
    // insertIntoBST(head, );

    return 0;
}