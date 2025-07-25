#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <stack>
#include <algorithm>

struct Node 
{
public:
    int data;
    struct Node *left;
    struct Node *right;

public:
    Node(int _data) : data(_data), left(nullptr), right(nullptr) {}
};

/* TRAVERSAL TECHNIQUES
1. BFS
2. DFS

DEPTH FIRST SEARCH

        1
       /  \
      2    3
    /  \   / \
   4    5 6   7

        1
       /  \
      2    3
    /  \   / \
   4    5 6   7
       /     / \
      8     9   10
1. Inorder Traversal (Left Root Right)
    4 2 5 1 6 3 7
    4 2 8 5 1 6 3 9 7 10
2. Pre-order Traversal ( Root Left Right)
    1 2 4 5 3 6 7
    1 2 4 5 8 3 6 7 9 10
3. Post-Order Traversal (Left Right Root)
    4 5 2 6 7 3 1
    4 8 5 2 6 9 10 7 3 1

Breath First Traversal
    1 2 3 4 5 6 7
    1 2 3 4 5 6 7 8 9 10
 */


// Pre-order Traversal ( Root Left Right)
/*  TC = O(N)
    SC = O(N)
*/
void preorder(struct Node *node) {
    if (node == NULL) return;

    std::cout << node->data << " ";

    preorder(node->left);
    preorder(node->right);
}

// Inorder Traversal (Left Root Right)
void InOrder(struct Node *node) {
    if (node == NULL) return;

    InOrder(node->left);
    std::cout << node->data << " ";
    InOrder(node->right);
}

// Post-Order Traversal (Left Right Root)
void PostOrder(struct Node *node) {
    if (node == NULL) return;

    PostOrder(node->left);
    PostOrder(node->right);
    std::cout << node->data << " ";
}

/* LEVEL ORDER TRAVERSAL */
class LevelOrderClass
{
public: 
    std::vector<std::vector<int>> levelOrder(struct Node *root) {
        std::vector<std::vector<int>> ans;
        if (root == NULL) return ans;
        std::queue<Node*> q;
        q.push(root);

        while(!q.empty()) {
            int size = q.size();
            std::vector<int> level;
            for (int i=0; i<size; i++) {
                Node *node = q.front();
                q.pop();

                if (node->left != NULL) q.push(node->left);
                if (node->right != NULL) q.push(node->right);
                level.push_back(node->data);
            }
            ans.push_back(level);
        }
        return ans;
    }
};

class LevelOrderClassOptmized {
public:
    std::vector<std::vector<int>> levelOrder(struct Node *root) {
        if (root == NULL) return {}; // Early exit for empty tree

        std::vector<std::vector<int>> ans;
        std::queue<Node*> q;
        q.push(root);

        while (!q.empty()) {
            int size = q.size();
            std::vector<int> level;
            level.reserve(size); // Reserve space to avoid reallocations

            for (int i = 0; i < size; ++i) {
                Node *node = q.front();
                q.pop();

                // Push left and right children if they exist
                if (node->left) q.push(node->left);
                if (node->right) q.push(node->right);

                level.push_back(node->data); // Add current node's data
            }
            ans.push_back(std::move(level)); // Move `level` to avoid unnecessary copy
        }

        return ans;
    }
};

std::vector<int> iterative_preorder_traversal(struct Node *root) {
    if (root == NULL) return {};

    std::vector<int> ans;
    std::stack<Node*> st;
    st.push(root);

    while(!st.empty()) {
        // int size = st.size();
        Node *node = st.top();
        st.pop();
        ans.push_back(node->data);

        if (node->right) st.push(node->right);
        if (node->left)  st.push(node->left);
    }
    return ans;
}

std::vector<int> iterative_InOrder_traversal(struct Node *root) {
    if (root == NULL) return {};

    std::vector<int> ans;
    std::stack<Node*> st;
    Node *node = root;

    while(true) {
        if (node != NULL) {
            st.push(node);
            node = node->left;
        }
        else {
            if(st.empty()) break;
            node = st.top();
            st.pop();
            ans.push_back(node->data);
            node = node->right;
        }
    }
    return ans;
}

std::vector<int> iterative_PostOrder_traversal(struct Node *root) {
    if (root == NULL) return {};

    std::vector<int> ans;
    std::stack<Node*> st1, st2;
    st1.push(root);

    while(!st1.empty()) {
        root = st1.top();
        st1.pop();
        st2.push(root);
        if (root->left)  st1.push(root->left);
        if (root->right) st1.push(root->right);
    }

    while(!st2.empty()) {
        ans.push_back(st2.top()->data);
        st2.pop();
    }

    return ans;
}

/*
    TC = O(2N)
    SC = O(N)
*/
std::vector<int> iterative_PostOrder_traversal_Optimized(struct Node *root) {
    if (root == NULL) return {};

    std::vector<int> ans;
    std::stack<Node*> st;
    Node *cur = root;

    while (cur != NULL || !st.empty()) {
        if (cur != NULL) {
            st.push(cur);
            cur = cur->left;
        }
        else {
            Node* temp = st.top()->right;
            
            if (temp == NULL) {
                temp = st.top();
                st.pop();
                ans.push_back(temp->data);

                while (!st.empty() && temp == st.top()->right) {
                    temp = st.top();
                    st.pop();
                    ans.push_back(temp->data);
                }
            }
            else cur = temp;
        }
    }
    return ans;
}

std::vector<int> iterativePostOrderTraversalOptimized(struct Node *root) {
    if (root == NULL) return {}; // If the tree is empty, return an empty vector

    std::vector<int> ans;         // To store the postorder traversal
    std::stack<Node*> st;         // Stack for iterative traversal
    Node *prev = NULL;            // To track the previously processed node

    st.push(root);                // Start with the root node

    while (!st.empty()) {
        Node *cur = st.top();     // Peek the top node in the stack

        // If traversing down the tree
        if (!prev || prev->left == cur || prev->right == cur) {
            if (cur->left) {
                st.push(cur->left); // Push left child if it exists
            } else if (cur->right) {
                st.push(cur->right); // Push right child if it exists
            } else {
                ans.push_back(cur->data); // Leaf node, process it
                st.pop();
            }
        }
        // If traversing up from the left child
        else if (cur->left == prev) {
            if (cur->right) {
                st.push(cur->right); // Push right child if it exists
            } else {
                ans.push_back(cur->data); // Process the current node
                st.pop();
            }
        }
        // If traversing up from the right child
        else if (cur->right == prev) {
            ans.push_back(cur->data); // Process the current node
            st.pop();
        }

        prev = cur; // Update the previously processed node
    }

    return ans; // Return the postorder traversal result
}

int Maximum_Depth_BT(Node *root) {
    if (!root) return 0;

    int lh = Maximum_Depth_BT(root->left);
    int rh = Maximum_Depth_BT(root->right);

    return 1 + std::max(lh, rh);
}

// Check for Balanced Binary Tree 
int main() {
/*
        1
       /  \
      2    3
    /  \   / \
   4    5 6   7
       /     / \
      8     9   10
*/
    struct Node *root = new Node(1);
    root->left = new Node(2); 
    root->left->left = new Node(4); 
    root->left->right = new Node(5); 
    root->left->right->left = new Node(8); 

    root->right = new Node(3);
    root->right->left = new Node(6);
    root->right->right = new Node(7);
    root->right->right->left = new Node(9);
    root->right->right->right = new Node(10);

    preorder(root);
    std::cout << "\n";

    InOrder(root);
    std::cout << "\n";

    PostOrder(root);
    std::cout << "\n";

    LevelOrderClass obj;
    std::vector<std::vector<int>> ans = obj.levelOrder(root);

    for (auto it : ans) {
        for (auto it2 : it) {
            std::cout << it2 << " "; 
        }
    }
    std::cout << "\n";

    std::cout << "iterative_preorder_traversal\n";
    std::vector<int> ans2 = iterative_preorder_traversal(root);
    for (auto it : ans2) {
        std::cout << it << " "; 
    }
    std::cout << "\n";

    std::cout << "iterative_InOrder_traversal\n";
    std::vector<int> ans3 = iterative_InOrder_traversal(root);
    for (auto it : ans3) {
        std::cout << it << " "; 
    }
    std::cout << "\n";

    std::cout << "iterative_PostOrder_traversal\n";
    std::vector<int> ans4 = iterative_PostOrder_traversal(root);
    for (auto it : ans4) {
        std::cout << it << " "; 
    }
    std::cout << "\n";

    std::cout << "Optimize iterative_PostOrder_traversal\n";
    std::vector<int> ans5 = iterative_PostOrder_traversal_Optimized(root);
    for (auto it : ans5) {
        std::cout << it << " "; 
    }
    std::cout << "\n";

    std::cout << "Optimize iterative_PostOrder_traversal\n";
    std::vector<int> ans6 = iterativePostOrderTraversalOptimized(root);
    for (auto it : ans6) {
        std::cout << it << " "; 
    }
    std::cout << "\n";

    return 0;
}