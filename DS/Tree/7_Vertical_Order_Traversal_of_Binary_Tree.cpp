#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <map>
#include <set>

using namespace std;

struct Node {
    int data;
    Node *left;
    Node *right;

    explicit Node(int data) : data(data), left(nullptr), right(nullptr) {}
};

vector<vector<int>> verticalTraversal(Node *root) {
    map<int, map<int, multiset<int>>> nodes;
    queue<pair<Node*, pair<int, int>>> todo;
    todo.push({root, {0, 0}});
    
    while(!todo.empty()) {
        auto p = todo.front();
        todo.pop();

        Node *node = p.first;
        int x = p.second.first, y = p.second.second;
        nodes[x][y].insert(node->data);

        if (node->left) {
            todo.push({node->left, {x-1, y+1}});
        }
        if (node->right) {
            todo.push({node->right, {x+1, y+1}});
        }
    }

    vector<vector<int>> ans;
    for (auto p : nodes) {
        vector<int> col;
        for (auto q : p.second) {
            col.insert(col.end(), q.second.begin(), q.second.end());
        }
        ans.push_back(col);
    }
    return ans;
}

/* Right/Left View of Binary Tree */

void rightView(Node *node, int level, vector<int> &ans) {
    if (!node) return;

    if (level == ans.size()) ans.push_back(node->data);

    rightView(node->right, level+1, ans);
    rightView(node->left, level+1, ans);
}

vector<int> rightSideView(Node* root) {
    vector<int> ans; 
    rightView(root, 0, ans);
    return ans;   
}


bool Symmetric(Node* leftSub, Node* rightSub) {
    if (leftSub == nullptr && rightSub == nullptr) return true;
    if (leftSub == nullptr || rightSub == nullptr || leftSub->data != rightSub->data) return false;

    return Symmetric(leftSub->left, rightSub->right) && 
           Symmetric(leftSub->right, rightSub->left);  // Fixed this line
}

bool isSymmetric(Node* root) {
    if (!root) return true;
    return Symmetric(root->left, root->right);
}

/* Print Root to Node Path in Binary Tree */




/* L27 Lowest Common Ancestor of a Binary Tree 
*/
Node* lowestCommonAncestor(Node* root, Node* p, Node* q) {
    if (!root || p == root || q == root) return root;

    Node *left = lowestCommonAncestor(root->left, p, q);
    Node *right = lowestCommonAncestor(root->right, p, q);

    // if (left != NULL) {
    //     return right;
    // }
    // else if(right == NULL) {
    //     return left;
    // }
    // else { // Both left and right are not null we found our result
    //     return root;
    // }

    if (left != NULL && right != NULL) {
        return root; // If both left and right are non-null, root is the LCA
    } 
    return left != NULL ? left : right; // Return the non-null child (if any)
}


/* L28 Maximum Width of Binary Tree 
Width => no of nodes between any 2 nodes (In a level)

So flow level order triversal
number the nodes in each level
lastNode - firstNode + 1

when zero base indexing
left:  2i + 1
right: 2i + 2
*/
int widthOfBinaryTree(Node* root) {
    if (!root) return 0;

    int ans = 0;
    queue<pair<Node*, int>> q;
    q.push({root, 0});

    while(!q.empty()) {
        
    }
}

int main() {


    return 0;
}