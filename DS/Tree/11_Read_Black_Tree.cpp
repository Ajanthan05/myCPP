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



/* 
It is self balancing BST
Every node is either black or read
Root is always Black
Every leave which is Nil is black
If node is red then its children are black
Every path from a node to any of its descendent NIL node has same no of black node
*/

/* INSERT
1. if tree is empty, create new node as
    root node woth color black
2. If tree is not empty, create newnode as 
    leaf node with color Red.
3. If the parent of  newnode is black then exit.
4. if parent of newnode is red, then check the 
    color of parent's sibling of newnode
    a) If color is black or null then do sutable 
    rotation & recolor
    b) If color is RED then recolor & also check 
    if parent's present of newnode is not root node
    ten recolor it & recheck
*/

int main() {

    Node* head = new Node(6);
    // insertIntoBST(head, 3);
    // insertIntoBST(head, 8);
    // insertIntoBST(head, 7);
    // insertIntoBST(head, 2);
    // insertIntoBST(head, 5);
    // insertIntoBST(head, 4);
    // insertIntoBST(head, );
    // insertIntoBST(head, );

    return 0;
}