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

/* L37. Morris Traversal | Preorder | Inorder 
Thread binary tree
TC = O(N) + a(N)
SC = O(1)

last node then go back to root

1) left->null  
print(root)
=>right

2) before moving left 
roght most guy on left sub tree will be connected to root
then cur go to left
*/
vector<int> inorderTraversal(Node* root) {
    vector<int> inorder;
    Node *cur = root;
    if (cur->left != NULL) {
        inorder.push_back(cur->val);
        cur = cur->right;
    }
    else {
        Node  *prev = cur->left;
        // if it has right && its not pointing himself
        while (prev->right && prev->right != cur) {
            prev = prev->right;
        }
        if (prev->right == NULL) {
            prev->right = cur;  // Create a thread
            cur = cur->left;
        }
        else { // When it came back to root
            prev->right = NULL;
            inorder.push_back(cur->val);
            cur = cur->right;
        }
    }
    return inorder;
}



int main() {


    return 0;
}