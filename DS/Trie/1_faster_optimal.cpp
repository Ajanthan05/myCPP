#include <iostream>
#include <array>
#include <climits>
#include <string>

// ofstream
#include <fstream>

using namespace std;


struct TrieNode {
    TrieNode* children[26]{};
    bool isEndOfWord = false;

    TrieNode() = default;

    inline bool contains(char c) const {
        return children[c - 'a'] != nullptr;
    }

    inline void put(char c, TrieNode* node) {
        children[c - 'a'] = node;
    }

    inline TrieNode* get(char c) const {
        return children[c - 'a'];
    }

    inline void setEnd() {
        isEndOfWord = true;
    }

    inline bool isEnd() const {
        return isEndOfWord;
    }
};

class Trie {
private:
    TrieNode* root;

public:
    Trie() {
        root = new TrieNode();
    }

    void insert(const std::string& word) {
        TrieNode* node = root;
        for (char c : word) {
            if (!node->contains(c)) {
                node->put(c, new TrieNode());
            }
            node = node->get(c);
        }
        node->setEnd();
    }

    bool search(const std::string& word) const {
        TrieNode* node = root;
        for (char c : word) {
            if (!node->contains(c)) return false;
            node = node->get(c);
        }
        return node->isEnd();
    }

    bool startsWith(const std::string& prefix) const {
        TrieNode* node = root;
        for (char c : prefix) {
            if (!node->contains(c)) return false;
            node = node->get(c);
        }
        return true;
    }

    ~Trie() {
        clear(root);
    }

private:
    void clear(TrieNode* node) {
        if (!node) return;
        for (int i = 0; i < 26; ++i) {
            clear(node->children[i]);
        }
        delete node;
    }
};


/* With array */
class TrieNode {
public:
    std::array<TrieNode*, 26> children{};
    bool isEndOfWord = false;

    TrieNode() = default;

    inline bool contains(char c) const {
        return children[c - 'a'] != nullptr;
    }

    inline void put(char c, TrieNode* node) {
        children[c - 'a'] = node;
    }

    inline TrieNode* get(char c) const {
        return children[c - 'a'];
    }

    inline void setEnd() {
        isEndOfWord = true;
    }

    inline bool isEnd() const {
        return isEndOfWord;
    }
};

class Trie {
private:
    TrieNode* root;

public:
    Trie() {
        root = new TrieNode();
    }

    void insert(const std::string& word) {
        TrieNode* node = root;
        for (char c : word) {
            if (!node->contains(c)) {
                node->put(c, new TrieNode());
            }
            node = node->get(c);
        }
        node->setEnd();
    }

    bool search(const std::string& word) const {
        TrieNode* node = root;
        for (char c : word) {
            if (!node->contains(c)) return false;
            node = node->get(c);
        }
        return node->isEnd();
    }

    bool startsWith(const std::string& prefix) const {
        TrieNode* node = root;
        for (char c : prefix) {
            if (!node->contains(c)) return false;
            node = node->get(c);
        }
        return true;
    }

    ~Trie() {
        clear(root);
    }

private:
    void clear(TrieNode* node) {
        if (!node) return;
        for (TrieNode* child : node->children) {
            clear(child);
        }
        delete node;
    }
};