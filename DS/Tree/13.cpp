#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <utility>
#include <algorithm>

using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;

    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    explicit TreeNode(int val) : val(val), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

// Maximum Depth of Binary Tree
int maxDepth(TreeNode* root) {
    if(root == nullptr) return 0;
    
    int lh = maxDepth(root->left);
    int rh = maxDepth(root->right);

    return 1 + max(lh, rh);
}

// Children Sum Property in Binary Tree: Root Equals Sum of Children


void changeTree(TreeNode* root) {
    if(root == nullptr) return;

    int child = 0;
    if(root->left) child += root->left->val;
    if(root->right) child += root->right->val;

    if(child >= root->val) root->val = child;
    else {
        if(root->left) root->left->val = root->val;
        else if(root->right) root->right->val = root->val;
    }

    changeTree(root->left);
    changeTree(root->right);

    int tot = 0;
    if(root->left) tot += root->left->val;
    if(root->right) tot += root->right->val;
    if(root->left || root->right) root->val = tot;
}

// Print all the Nodes at a distance of K in Binary Tree
class Solution {
private:
    void markParents(TreeNode* root, TreeNode* target, unordered_map<TreeNode*, TreeNode*> &parent_track) {
        queue<TreeNode*> q;
        q.push(root);
        while(!q.empty()) {
            TreeNode *cur = q.front();
            q.pop();
            if(cur->left) {
                parent_track[cur->left] = cur;
                q.push(cur->left);
            }
            if(cur->right) {
                parent_track[cur->right] = cur;
                q.push(cur->right);
            }
        }
    }
public:
    vector<int> distanceK(TreeNode* root, TreeNode* target, int k) {
        unordered_map<TreeNode*, TreeNode*> parent_track;
        markParents(root, target, parent_track);
    }
};


// Note repeat: 14, 30