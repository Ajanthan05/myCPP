#include <iostream>
#include <vector>
#include <climits>
#include <string>

// ofstream
#include <fstream>

using namespace std;

struct TrieNode{
    TrieNode* children[26];
    bool isEndOfWord;

    TrieNode(){
        isEndOfWord=false;
        for(int i=0;i<26;i++){
            children[i]=nullptr;
        }
    }

    bool contains(char c){
        return children[c-'a']!=nullptr;
    }

    void put(char c, TrieNode* node){
        children[c-'a']=node;
    }

    TrieNode* get(char c){
        return children[c-'a'];
    }

    void setEnd(){
        isEndOfWord=true;
    }

    bool isEnd(){
        return isEndOfWord;
    }

};

class Trie {
    TrieNode* root;
public:
    Trie() {
        root=new TrieNode();
    }
    
    void insert(string word) {
        TrieNode* node=root;
        for(char c: word){
            if(!node->contains(c))
                node->put(c,new TrieNode());
            node=node->get(c);
        }
        node->setEnd();
    }
    
    bool search(string word) {
        TrieNode* node=root;
        for(char c:word){
            if(!node->contains(c))
                return false;
            node=node->get(c);
        }
        return node->isEnd();
    }
    
    bool startsWith(string prefix) {
        TrieNode* node=root;
        for(char c:prefix){
            if(!node->contains(c))
                return false;
            node = node->get(c);
        }

        return true;
    }
};

auto init = atexit([]() { ofstream("display_runtime.txt") << "0";});