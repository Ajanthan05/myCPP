#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>

using namespace std;

/* Diameter of Binary Tree
    Longest path between 2 nodes
    Path does not need to pass via root
*/

struct Node {
    int data;
    Node *left;
    Node *right;

    explicit Node(int data) : data(data), left(nullptr), right(nullptr) {}
};

int findMax(Node *node, int &maxi) {
    if(!node) return 0;

    int lh = findMax(node->left, maxi);
    int rh = findMax(node->right, maxi);

    maxi = max(maxi, lh + rh);
    
    return 1 + max(lh, rh);
}

int Diameter(Node *node) {
    int maxi = 0;
    findMax(node, maxi);
    return maxi;
}

/* Maximum Path Sum in Binary Tree */
int maxPathDown(Node *node, int &maxi) {
    if(!node) return 0;

    // int lSum = maxPathDown(node->left, maxi);
    // int rSum = maxPathDown(node->right, maxi);

    // int tmpMax = node->data;
    // if (lSum > 0) tmpMax += lSum;
    // if (rSum > 0) tmpMax += rSum;
    // maxi = max(maxi, tmpMax);

    int lSum = max(0, maxPathDown(node->left, maxi));
    int rSum = max(0, maxPathDown(node->right, maxi));
    maxi = max(maxi, lSum + rSum + node->data);
    
    return node->data + max(lSum, rSum);
}

int MaxPathSum(Node *node) {
    int maxi = 0;
    maxPathDown(node, maxi);
    return maxi;
}

/* Check it two trees are Identical or Not */
bool isSameTree(Node *p, Node *q) {
    if (p == NULL || q == NULL) return (p==q);

    return (p->data == q->data)
        && isSameTree(p->left, q->left)
        && isSameTree(p->right, q->right);
}

/* Zig-Zag or Spiral Traversal in Binary Tree */

vector<vector<int>> xigZagLevelOrder(Node *root) {
    vector<vector<int>> result;
    if (!root) return result;

    queue<Node*> nodesQueue;
    nodesQueue.push(root);
    bool leftToRight = true;

    while(!nodesQueue.empty()) {
        int size = nodesQueue.size();
        vector<int> row(size);

        for(int i=0; i<size;i++) {
            Node *node = nodesQueue.front();
            nodesQueue.pop();

            // Find the position to fill nod's val
            int index = (leftToRight) ? i : (size -1 - i);
            row[index] = node->data;

            if (node->left) nodesQueue.push(node->left);
            if (node->right) nodesQueue.push(node->right);
        }
        leftToRight = !leftToRight;
        result.push_back(row);
    }
    return result;
}

/* L20  Boundary Traversal in Binary Tree 
1. left boundry excluding leafe
2. leave node  (Inorder triversal)
3. Right boundry in the reverse order (stack)
*/
 
int main() {


    return 0;
}