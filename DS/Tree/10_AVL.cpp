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

// BST, AVL, BINARY, b TFREE, B+ TREE, HEAP TREE

/* AVL Tree
it is a BST
| height of left subtree - height of right subtree | = {-1, 0, 1}
Balance factor -1 or 0 or 1
AVL Tree is strictely height balance tree
Duplicate elements are not allowed in BST
*/

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