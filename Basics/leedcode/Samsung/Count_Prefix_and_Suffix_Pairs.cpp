#include <bits/stdc++.h>
using namespace std;


int countPrefixSuffixPairs(vector<string>& words) {
    int n = words.size();
    int ans = 0;
    for(int i=0; i<n; ++i) {
        for(int j=i+1; j<n; ++j) {
            if (words[i] == words[j].substr(0, words[i].size()) && words[i] == words[j].substr(words[j].size()-words[i].size())) {
                ans++;
            }
        }
    } 
    return ans;       
}

#define ll long long int
class TrieNode {
public:
    unordered_set<int, TrieNode*> children;
    int count;

    TrieNode() {
        count = 0;
    }
};

class Solution {
public:
    TrieNode* root;
    Solution() {
        root = new TrieNode();
    }
}

int main() {
    vector<string> a = {"a","aba","ababa","aa"};
    cout << countPrefixSuffixPairs(a) << "\n";

    vector<string> b = {"aa", "a","aba","ababa"};
    cout << countPrefixSuffixPairs(b) << "\n";
}