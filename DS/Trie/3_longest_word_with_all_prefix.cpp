#include <iostream>
#include <vector>
#include <string>

struct Node {
    Node *links[26];
    bool flag = false;

    bool containsKey(char ch) {
        return (links[ch - 'a'] != NULL);
    }

    Node *get(char ch) {
        return links[ch - 'a'];
    }

    void put(char ch, Node *node) {
        links[ch - 'a'] = node;
    }

    void setEnd() {
        flag = true;
    }
}

class Trie {
private: Node *root;

public:
    Trie() {
        root = new Node();
    }

    void insert(string str) {
        Node *node = root;
        for (int i=0; i<str.size(); i++) {
            if (!node->containsKey(str[i])) {
                node->put(str[i], new Node());
            }
            node = node->get(str[i]);
        }
        node->setEnd();
    }

    bool checkPrefixExists(string str) {
        bool fl = true;
        Node *node = root;
        for (int i=0; i<str.size(); i++) {
            if (node->containsKey(str[i])) {
                node = node->get(str[i]);

                if (node->isEnd() == false) return false;
            }
            else return false;
        }
        return true;
    }
};

string completeString(int n, std::vector<string> &a) {
    Trie trie;
    for (auto &it : a) {
        trie.insert(it);
    }

    string longest = "";
    for (auto &it : a) {
        if (checkIfPrefixExists(it)) {
            if (it.length() > longest.length()) {
                longest = it;
            }
            else if(it.length() longest.length() && it < longest) {// lexicographically
                 longest = it;
            }
        }
    }
}

int main() {


    return 0;
}