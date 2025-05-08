#include <iostream>
#include <unordered_map>

struct Node {
    int key;
    int value;
    int cnt;
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
