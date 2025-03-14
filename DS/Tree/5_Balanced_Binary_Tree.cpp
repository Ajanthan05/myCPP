#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <cstdlib>
#include <cmath>

using namespace std;

struct Node {
public:
    int data;
    Node *left;
    Node *right;

    Node(int data) : data(data), left(nullptr), right(nullptr) {}
};

int height(Node *node) {
    if(node == NULL) return 0;

    int lh = height(node->left);
    int rh = height(node->right);

    return max(lh, rh) + 1;
}
int height_O(Node *node) {
    if (!node) return 0;
    return std::max(height(node->left), height(node->right)) + 1;
}

int checkBalance(Node *node) {
    if(node == NULL) return 0;

    int lh = checkBalance(node->left);
    if (lh == -1) return -1;
    int rh = checkBalance(node->right);
    if (rh == -1) return -1;

    if (abs(lh - rh) > 1) return -1;
    return max(lh, rh) + 1;
}

bool isBalanced(Node *root) {
    return checkBalance(root) != -1;
}

int main() {

    Node *root = new Node(1);
    root->left = new Node(2);
    root->right = new Node(3);
    root->left->left = new Node(4);
    root->left->right = new Node(5);
    root->left->left->left = new Node(6);

    cout << (isBalanced(root) ? "Balanced" : "Not Balanced") << endl;
    
    return 0;
}