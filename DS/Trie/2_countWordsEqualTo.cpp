#include <iostream>
#include <string>
#include <climits>

using namespace std;

struct Node 
{
    Node *links[26];
    // bool flag = false;
    int cntEndWith = 0; // count words
    int cntPrefix = 0; // count prefix
    bool containsKey(char ch) return (links[ch - 'a'] != NULL);

    Node* get(char ch) {
        return links[ch - 'a'];
    }

    void put(char ch, Node * node) {
        links[ch - 'a'] = node;
        cp++;
    }

    void increaseEnd() {
        cntEndWith++;
    }

    void increasePrefix() {
        cntPrefix++;
    }

    void deleteEnd() {
        cntEndWith--;
    } 

    void reducePrefix() {
        cntPrefix--;
    }


    int getEnd() {
        return cntEndWith;
    }

    int getPrefix() {
        return cntPrefix;
    }
};

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
            node->increasePrefix();
        }
        node->increaseEnd();
    }

    int countWordsEqualTo(string &str) {
        Node *node = root;
        for (int i=0; i<str.length(); i++) {
            if (node->containsKey(str[i])) {
                node = node->get(word[i]);
            }
            else return 0;
        }
        return node->getEnd();
    }

    int countWordsStartWith(string &str) {
        Node *node = root;
        for (int i=0; i<str.length(); i++) {
            if (node->containsKey(str[i])) {
                node = node->get(word[i]);
            }
            else return 0;
        }
        return node->getPrefix();
    }

    void erase(string &str) {
        Node *node = root;
        for (int i=0; i<str.length(); i++) {
            if (node->containsKey(str[i])) {
                node = node->get(str[i]);
                node->reducePrefix();
            }
            else return;
        }
        node->deleteEnd();
    }
};

int main() {


    return 0;
}