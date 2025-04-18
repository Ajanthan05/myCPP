#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>

using namespace std;

struct Node {
    Node* links[26];

    bool isContainKey(char ch) {
        return links[ch - 'a'];
    }

    void put(char ch, Node* node) {
        links[ch - 'a'] = node;
    }

    Node* get(char ch) {
        return links[ch - 'a'];
    }
};

int Counte(const std::string &s) {
    int cnt = 0;
    Node* root = new Node();
    int n = s.size();

    for(int i=0; i<n; i++) {
        Node* node = root;
        for(int j=i; j<n; j++) {
            if (!node->isContainKey(s[j])) {
                cnt++;
                node->put(s[j], new Node());
            }
            node = node->get(s[j]);
        }
    }
    return cnt + 1;
}

int main() {


    return 0;
}