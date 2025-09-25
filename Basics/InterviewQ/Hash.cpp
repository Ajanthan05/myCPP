#include <vector>
#include <functional>
#include <stdexcept>
#include <limits>
#include <utility>

using namespace std;

/*  In C++, the hash class is default constructible function class (functor) that provides the default hash function used by STL. It is used to get the hash value of the argument that is being passed to it. If the argument doesn't change, the value doesn't change either.*/

void BitSet() {
    bitset<5> b("10101");

    // Create an object of hash class
    hash<bitset<5>> h;

    // Generate the hash value of given bitset
    cout << h(b) << "\n";
}
void T() {
    string s = "geeksforgeeks";

    hash<string> h;

    // Get the hash value of character
    cout << h(s) << "\n";

    vector<bool> v = {true, false};

    // Create an object of hash class
    hash<vector<bool>> h2;

    // Generate the hash value of given vector
    cout << h2(v) << endl;
}

// Generate the Hash Value of Custom Data Type
struct A {
    int a;
  	A(int x = 0): a(x) {}
};
namespace std {
    template <>
    struct hash<A> {
        size_t operator()(const A& p) const {
            return hash<int>{}(p.a);
        }
    };
}
void T2() {
    A obj(11);

    hash<A> h;
    cout << h(obj) << "\n";
}
/*
template <typename KeyType, typename ValueType>
class OpenAdddressingHashTable {
private:
    struct Node {
        KeyType key_;
        ValueType value_;
        bool is_occupied = false;
    };
    std::vector<Node> table_;
    size_t current_size_;
    size_t capacity_;

    size_t Hash(const KeyType& key) const {
        return std::hash<KeyType>{}(key)%capacity_;
    }

    void expand() {
        size_t old_capacity = capacity_;

        if (capacity_ > numeric_limits<size_t>::max() / 2) {
            throw logic_error("Capacity reached");
        }
        capacity *= 2;
        std::vector<Node> old_table = table_;
        table_.clear();
        table_.resize(capacity_);
        current_size_ = 0;

        for (const auto& node : old_table) {
            if (node.is_occupied)
        }
    }

public:
    OpenAdddressingHashTable(size_t initial_capacity = 16) : 
                                capacity_(initial_capacity,), current_size_(0) {
                                    table_.reserve(capacity_);
                                }

    void insert(const KeyType& key, const ValueType& value) {}

    bool contains(const KeyType& key) {
        size_t index = Hash(key);
        size_t start_index = inxex;

        while (table_[index].is_occupied_) {
            if (table_[index].key_ == key) {
                return true;
            }
            index = (index + 1) % capacity_;
        }

        return false;
    }

    size_t size() const {
        return current_size_;
    }
    size_t capacity() const {
        return capacity_;
    }
};
*/



template <typename KeyType, typename ValueType>
class OpenAddressingHashTable {
private:
    struct Node {
        KeyType key_;
        ValueType value_;
        bool is_occupied = false;
        bool is_deleted = false;  // tombstone marker for deletions
    };

    std::vector<Node> table_;
    size_t current_size_;
    size_t capacity_;

    // Load factor threshold before resizing
    static constexpr double kMaxLoadFactor = 0.7;

    size_t Hash(const KeyType& key) const {
        return std::hash<KeyType>{}(key) % capacity_;
    }

    void expand() {
        if (capacity_ > std::numeric_limits<size_t>::max() / 2) {
            throw std::logic_error("Capacity overflow");
        }

        size_t old_capacity = capacity_;
        capacity_ *= 2;
        std::vector<Node> old_table = std::move(table_);

        table_.clear();
        table_.resize(capacity_);
        current_size_ = 0;

        for (const auto& node : old_table) {
            if (node.is_occupied && !node.is_deleted) {
                insert(node.key_, node.value_);
            }
        }
    }

public:
    OpenAddressingHashTable(size_t initial_capacity = 16)
        : current_size_(0), capacity_(initial_capacity) {
        table_.resize(capacity_);
    }

    void insert(const KeyType& key, const ValueType& value) {
        if ((double)current_size_ / capacity_ > kMaxLoadFactor) {
            expand();
        }

        size_t index = Hash(key);

        while (table_[index].is_occupied && !table_[index].is_deleted) {
            if (table_[index].key_ == key) {
                // Update existing value
                table_[index].value_ = value;
                return;
            }
            index = (index + 1) % capacity_;
        }

        // Insert new node
        table_[index].key_ = key;
        table_[index].value_ = value;
        table_[index].is_occupied = true;
        table_[index].is_deleted = false;
        ++current_size_;
    }

    bool contains(const KeyType& key) const {
        size_t index = Hash(key);
        size_t start_index = index;

        while (table_[index].is_occupied) {
            if (!table_[index].is_deleted && table_[index].key_ == key) {
                return true;
            }
            index = (index + 1) % capacity_;
            if (index == start_index) break; // full cycle
        }
        return false;
    }

    bool erase(const KeyType& key) {
        size_t index = Hash(key);
        size_t start_index = index;

        while (table_[index].is_occupied) {
            if (!table_[index].is_deleted && table_[index].key_ == key) {
                table_[index].is_deleted = true; // mark as tombstone
                --current_size_;
                return true;
            }
            index = (index + 1) % capacity_;
            if (index == start_index) break;
        }
        return false;
    }

    ValueType* find(const KeyType& key) {
        size_t index = Hash(key);
        size_t start_index = index;

        while (table_[index].is_occupied) {
            if (!table_[index].is_deleted && table_[index].key_ == key) {
                return &table_[index].value_;
            }
            index = (index + 1) % capacity_;
            if (index == start_index) break;
        }
        return nullptr;
    }

    size_t size() const { return current_size_; }
    size_t capacity() const { return capacity_; }
    bool empty() const { return current_size_ == 0; }
};


// https://www.geeksforgeeks.org/cpp/unordered_set-in-cpp-stl/
int main() {
    BitSet();
    T();
    T2();
    return 0;
}