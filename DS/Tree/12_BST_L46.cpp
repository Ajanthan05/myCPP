#include <iostream>
#include <vector>
#include <climits>
#include <stack>
#include <utility>
#include <algorithm>

using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;

    explicit TreeNode(int val) : val(val), left(nullptr), right(nullptr) {}
};

// bool inRange(TreeNode* root, int min, int max) {
//     if(!root) return true;

//     if(root->val >= max || root->val <= min) return false;

//     return inRange(root->left, max = root->val, max) && inRange(root->right, min, min = root->val);
// }

// bool isValidBST(TreeNode* root) {
//     return inRange(root, INT_MIN, INT_MAX);
// }


bool inRange(TreeNode* root, long long min, long long max) {
    if (!root) return true;

    if (root->val >= max || root->val <= min) return false;

    return inRange(root->left, min, root->val) && 
           inRange(root->right, root->val, max);
}

bool isValidBST(TreeNode* root) {
    return inRange(root, LLONG_MIN, LLONG_MAX);
}


/* 235. (LCA) Lowest Common Ancestor of a Binary Search Tree 
From the bottom first intersection Point
or
When both of them are in left or write move (left/right)

TC = O(H)
SC = O(1)
*/
TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
    if(!root) return root;

    while(root) {
        // if (root == p || root == q) return root;
        if (p->val < root->val && q->val < root->val) {
            root = root->left;
        }
        else if (p->val > root->val && q->val > root->val) {
            root = root->right;
        }
        else return root;
    }
    return root;
}

/* Construct a BST from a preorder traversal
BST => Inorder Traversal always be sorted
*/
TreeNode* build(vector<int> &preorder, int &ind, int upperBound, int n) {
    if (ind == n || preorder[ind] > upperBound) return NULL;
    TreeNode* root = new TreeNode(preorder[ind++]);
    root->left = build(preorder, ind, root->val, n);
    root->right = build(preorder, ind, upperBound, n);
    return root;
}
TreeNode* bstFromPreorder(vector<int>& preorder) {
    int i = 0;
    int n = preorder.size();
    return build(preorder, i, INT_MAX, n);
}

/* Avoid recursion stack space*/
TreeNode* bstFromPreorder(vector<int>& preorder) {
    if (preorder.empty()) return nullptr;
    
    stack<TreeNode*> st;
    TreeNode* root = new TreeNode(preorder[0]);
    st.push(root);

    for (int i = 1; i < preorder.size(); i++) {
        TreeNode* node = new TreeNode(preorder[i]);
        TreeNode* parent = nullptr;

        // Find the correct parent for the new node
        while (!st.empty() && preorder[i] > st.top()->val) {
            parent = st.top();
            st.pop();
        }
        // If we found a parent, it means we are inserting into the right subtree
        if (parent) {
            parent->right = node;
        } 
        // Otherwise, it belongs to the left subtree of the last node in the stack
        else {
            st.top()->left = node;
        }
        // Push the new node onto the stack
        st.push(node);
    }
    return root;
}

/* Inorder Successor/Predecessor in BST 

*/


/* Binary Search Tree Iterator */
class BSTIterator {
private:
    stack<TreeNode*> st;

    void pushAll(TreeNode *node) {
        for(; node != NULL; st.push(node), node = node->left);
    }

public:
    BSTIterator(TreeNode* root) {
        pushAll(root);
    }
    
    int next() {
        TreeNode *tmpNode = st.top();
        st.pop();
        pushAll(tmpNode->right);
        return tmpNode->val;
    }
    
    bool hasNext() {
        return !st.empty();
    }
};

/*  FOR REVERSE

*/
class BSTIterator_REV {
private:
    bool reverse; 
    stack<TreeNode*> st;

    void pushAll(TreeNode *node) {
        for(; node != NULL; ) {
            if (reverse) {
                st.push(node);
                node = node->right;
            }
            else {
                st.push(node);
                node = node->left;
            }
        }
    }

public:
    BSTIterator_REV(TreeNode* root, bool isReverse) {
        pushAll(root);
        reverse = isReverse;
    }
    
    int next() {
        TreeNode *tmpNode = st.top();
        st.pop();
        pushAll(tmpNode->right);
        return tmpNode->val;
    }

    int before() {
        TreeNode *tmpNode = st.top();
        
    }
    
    bool hasNext() {
        return !st.empty();
    }
};


/* Recover Binary Search Tree
You are given the root of a binary search tree (BST), where the values of exactly two nodes of the tree were swapped by mistake. Recover the tree without changing its structure.

*/
int main() {


    return 0;
}