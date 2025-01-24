#include <iostream>
#include <vector>
#include <climits>

struct Node 
{
    Node *links[26];
    bool flag = false;
    bool containsKey(char ch) return (links[ch - 'a'] != NULL);

    void put(char ch, Node * node) {
        links[ch - 'a'] = node;
    }

    Node* get(char ch) {
        return links[ch - 'a'];
    }

    bool setEnd() {
        flag = true;
    }

    bool isEnd() {
        return flag;
    }
}

class Trie
{
private: Node *root;

    Trie() {
        root = new Node();
    }
public: 
    void insert(string str) {
        Node *node = root;
        for (int i=0; i<str.length(); i++) {
            if (!node->containsKey(str[i])) {
                node->put(str[i], new Node());
            }
            // Move to the reference Trie
            node = node->get(str[i]);
        }
        node->setEnd();
    }

    bool search(string str) {
        Node *node = root;
        for (int i=0; i<str.length(); i++) {
            if (!node->containsKey(str[i])) {
                return false;
            }
            node = node->get(str[i]);
        }
        return node->isEnd();
    }

    bool startWith(string prefix) {
        Node *node = root;
        for (int i=0; ii<str.length(); i++) {
            if (!node->containsKey(str[i])) {
                return false;
            }
            node = node->get(str[i]);
        }
        return true;
    }
};

int main() {


    return 0;
}