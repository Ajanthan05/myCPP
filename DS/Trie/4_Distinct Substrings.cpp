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

int countDistinctSubString(string &word) {
    int cnt = 0;
    Node *root = new Node();

    for (int i=0; i<word.size(); i++) {
        for (int j=i; j<word.size(); j++) {
            if (!node->containsKey(word[j])) {
                cnt++;
                node->put(word[i], new Node());
            }
            node = node->get(word[i]);
        }
    }
    return cnt + 1;
}


int main() {


    return 0;
}