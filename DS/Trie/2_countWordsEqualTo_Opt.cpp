#include <iostream>
#include <array>
#include <climits>
#include <string>

using namespace std;

struct TrieNode {
    public:
    std::array<TrieNode*, 26> children{};
    int cntEndWith = 0;    
    int cntPrefix = 0;    

    TrieNode() = default;

    inline bool contains(char c) const {
        return children[c - 'a'] != nullptr;
    }

    inline void put(char ch, TrieNode* node) {
        children[ch - 'a'] = node;
    }

    inline TrieNode* get(char ch) const {
        return children[ch - 'a'];
    }

    inline void setEnd() {
        cntEndWith++;
    }

    inline bool isEnd(char ch) const {
        return cntEndWith != 0;
    }

    inline void increaseEnd() {
        cntEndWith++;
    }

    inline void increasePrefix() {
        cntPrefix++;
    }

    inline void deleteEnd() {
        cntEndWith--;
    }

    inline void reducePrefix() {
        cntPrefix-- ;
    }

    inline int getEnd() const {
        return cntEndWith;
    }

    inline int getPrefix() const {
        return cntPrefix;
    }
};

class Trie {
    TrieNode* root;

public:
    Trie() {
        root = new TrieNode();
    }

    void insert(const std::string& word) {
        TrieNode* node = root;
        for(char ch : word) {
            if(!node->contains(ch)) {
                node->put(ch, new TrieNode()); // put alone with empty node
            }
            node = node->get(ch);
            node->increasePrefix();
        }
        node->increaseEnd();
    }

    int countWordsEqualTo(const std::string& word) const {
        TrieNode* node = root;
        for(char ch : word) {
            if(node->contains(ch)) {
                node = node->get(ch);
            }
            else {
                return 0;
            }
        }
        return node->getEnd();
    }

    int countWordsStartingWidth(const std::string& word) const {
        TrieNode* node = root;
        for(char ch : word) {
            if(node->contains(ch)) {
                node = node->get(ch);
            }
            else {
                return 0;
            }
        }
        return node->getPrefix();
    }

    void erase(const std::string& word) {
        if (!countWordsEqualTo(word)) return;

        TrieNode* node = root;
        for(char ch : word) {
            node = node->get(ch);
            node->reducePrefix();
        }
        node->deleteEnd();
    }

    ~Trie() {
        clean(root);
    }

private:
    void clean(TrieNode* node) {
        if (!node) return;
        
        for(TrieNode* child : node->children) {
            clean(child);
        }
        delete node;
    }
};



/*
3. Avoid Redundant contains() Checks
You're doing:

cpp
Copy
Edit
if (!node->contains(ch)) {
    node->put(ch, new TrieNode());
}
node = node->get(ch);
Instead, just call get() and check:

cpp
Copy
Edit
TrieNode* next = node->get(ch);
if (!next) {
    next = new TrieNode();
    node->put(ch, next);
}
node = next;
It’s slightly more efficient, avoids double computation of ch - 'a'.

4. Avoid Repeating ch - 'a'
You can compute the index once:

cpp
Copy
Edit
int idx = ch - 'a';
Especially inside loops (micro-optimization, but worth it in tight loops).

5. isEnd(char ch) Misleading Name
The method name isEnd(char ch) is misleading because it doesn’t use ch and just returns cntEndWith != 0. Rename to just isEnd().

*/

struct TrieNode {
    std::array<TrieNode*, 26> children{};
    int cntEndWith = 0;
    int cntPrefix = 0;

    TrieNode() = default;

    inline bool contains(char c) const {
        return children[c - 'a'] != nullptr;
    }

    inline void put(char ch, TrieNode* node) {
        children[ch - 'a'] = node;
    }

    inline TrieNode* get(char ch) const {
        return children[ch - 'a'];
    }

    inline void increaseEnd() { cntEndWith++; }
    inline void increasePrefix() { cntPrefix++; }
    inline void deleteEnd() { cntEndWith--; }
    inline void reducePrefix() { cntPrefix--; }
    inline int getEnd() const { return cntEndWith; }
    inline int getPrefix() const { return cntPrefix; }
    inline bool isEnd() const { return cntEndWith != 0; }
};

class Trie {
public:
    Trie() {
        root = new TrieNode();
    }

    ~Trie() {
        clean(root);
    }

    void insert(const std::string& word) {
        TrieNode* node = root;
        for (char ch : word) {
            int idx = ch - 'a';
            if (!node->children[idx]) {
                node->children[idx] = new TrieNode();
            }
            node = node->children[idx];
            node->increasePrefix();
        }
        node->increaseEnd();
    }

    int countWordsEqualTo(const std::string& word) const {
        TrieNode* node = root;
        for (char ch : word) {
            node = node->get(ch);
            if (!node) return 0;
        }
        return node->getEnd();
    }

    int countWordsStartingWith(const std::string& prefix) const {
        TrieNode* node = root;
        for (char ch : prefix) {
            node = node->get(ch);
            if (!node) return 0;
        }
        return node->getPrefix();
    }

    void erase(const std::string& word) {
        if (!countWordsEqualTo(word)) return;

        TrieNode* node = root;
        for (char ch : word) {
            node = node->get(ch);
            node->reducePrefix();
        }
        node->deleteEnd();
    }

private:
    TrieNode* root;

    void clean(TrieNode* node) {
        if (!node) return;
        for (TrieNode* child : node->children) {
            clean(child);
        }
        delete node;
    }
};
