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

    return max(lh, rh);
}

int balanceBT(Node *node) {
    if(node == NULL) return 0;

    int lh = height(node->left);
    int rh = height(node->right);

    if (lh == -1 && rh == -1) return -1;
    if (abs(lh, rh) > 1) return -1;
    return max(lh, rh) + 1;
}

int main() {


    return 0;
}