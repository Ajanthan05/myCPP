#include <iostream>
#include <vector>
#include <climits>
#include <string>

using namespace std;

struct Node 
{
    Node *links[26];
    bool flag = false;

    bool containsKey(char ch) {
        return (links[ch - 'a'] != NULL);
    }
    
    void put(char ch, Node* node) {
        links[ch - 'a'] = node;
    }

    Node* get(char ch) {
        return links[ch - 'a'];
    }

    void setEnd() {
        flag = true;
    }

    bool isEnd() {
        return flag;
    }
};

class Trie
{
private: Node *root;

public: 
    Trie() {
        root = new Node();
    }

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
        for (int i=0; i<prefix.length(); i++) {
            if (!node->containsKey(prefix[i])) {
                return false;
            }
            node = node->get(prefix[i]);
        }
        return true;
    }
};

int main() {
    Trie trie;

    // Insert words into the Trie
    trie.insert("apple");
    trie.insert("app");
    trie.insert("bat");
    trie.insert("batman");

    // Search for words
    cout << "Search 'apple': " << (trie.search("apple") ? "Found" : "Not Found") << endl;
    cout << "Search 'app': " << (trie.search("app") ? "Found" : "Not Found") << endl;
    cout << "Search 'bat': " << (trie.search("bat") ? "Found" : "Not Found") << endl;
    cout << "Search 'batman': " << (trie.search("batman") ? "Found" : "Not Found") << endl;
    cout << "Search 'batmobile': " << (trie.search("batmobile") ? "Found" : "Not Found") << endl;

    // Check prefixes
    cout << "Starts with 'ap': " << (trie.startWith("ap") ? "Yes" : "No") << endl;
    cout << "Starts with 'batm': " << (trie.startWith("batm") ? "Yes" : "No") << endl;
    cout << "Starts with 'cat': " << (trie.startWith("cat") ? "Yes" : "No") << endl;

    return 0;
}
/*
Search 'apple': Found
Search 'app': Found
Search 'bat': Found
Search 'batman': Found
Search 'batmobile': Not Found
Starts with 'ap': Yes
Starts with 'batm': Yes
Starts with 'cat': No
*/