#include <iostream>

#include <unordered_map>
using namespace std;

// Least Recently Used (LRU) cache. 
/*
struct Node {
    int key, val;
    Node *next = nullptr, *prev = nullptr;
};

void insertAfterHead(Node *node, Node *head) {
    Node *curAfterHead = head->next;
    head->next = node;
    node->prev = head;
    node->next = curAfterHead;
}

void deletNode(Node *node) {
    Node *previous = node->prev;
    Node *after = node->next;
    previous->next = after;
    after->prev = previous;
}

class LRUCache {
    map<int, Node*> mpp;
    int capacity;
    Node *head = new Node();
    Node *tail = new Node();

    LRUCache(int capacity_) : capacity{capacity} {
        mpp.clear();
        head->next = tail;
        tail->prev = head;
    }

    int get(int key) {
        if (mpp.find(key) == mpp.end()) return -1;
        Node* node = mpp[key]; 
        deletNode(node);
        insertAfterHead(node);

        return node->val;
    }

    int put(int key, int value) {
        if (mpp.find(key) != mpp.end()) {
            Node* node = mpp[key];
            node->val = value;
            insertAfterHead(node);
        }
        else {
            if (mpp.size() == capacity) {
                Node* node = tail->prev;
                mpp.erase(node->key);
                deletNode(node);
            }
        }

        Node *node = new Node();
        mpp[key] = node;
        insertAfterHead(node);
    }
};
*/


struct Node {
    int key;
    int value;
    Node* prev;
    Node* next;

    Node(int k, int v) : key(k), value(v), prev(nullptr), next(nullptr) {}
};

class LRUCache {
public:
    LRUCache(int capacity_) : capacity(capacity_) {
        head = new Node(0, 0);  // dummy head
        tail = new Node(0, 0);  // dummy tail
        head->next = tail;
        tail->prev = head;
    }

    ~LRUCache() {
        // clean up all nodes
        Node* curr = head;
        while (curr) {
            Node* temp = curr;
            curr = curr->next;
            delete temp;
        }
    }

    int get(int key) {
        if (map.find(key) == map.end()) return -1;

        Node* node = map[key];
        remove(node);
        insertAfterHead(node);
        return node->value;
    }

    void put(int key, int value) {
        if (map.find(key) != map.end()) {
            // Update value and move to front
            Node* node = map[key];
            node->value = value;
            remove(node);
            insertAfterHead(node);
        } else {
            // Evict if needed
            if (map.size() == capacity) {
                Node* lru = tail->prev;
                map.erase(lru->key);
                remove(lru);
                delete lru;
            }
            Node* newNode = new Node(key, value);
            map[key] = newNode;
            insertAfterHead(newNode);
        }
    }

private:
    int capacity;
    std::unordered_map<int, Node*> map;
    Node* head;
    Node* tail;

    void remove(Node* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    void insertAfterHead(Node* node) {
        node->next = head->next;
        node->prev = head;
        head->next->prev = node;
        head->next = node;
    }
};

int main() {


    return 0;
}