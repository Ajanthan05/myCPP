#include <iostream>
#include <vector>
#include <iterator>

// Custom forward iterator over int*
class MyIterator {
public:
    using iterator_category = std::forward_iterator_tag; //iterator_category: Declares this as a forward iterator.
    using value_type        = int;  //value_type: The type of elements this iterator returns (int).
    using difference_type   = std::ptrdiff_t;  // difference_type: Type used for distances between iterators (ptrdiff_t).
    using pointer           = int*;  // pointer and reference: Usual iterator traits for working with STL.
    using reference         = int&;

    MyIterator(int* ptr) : p(ptr) {}

    /*  Dereference operator
    Allows you to access the value the iterator points to.
    Equivalent to *it for standard iterators.*/
    int& operator*() const { return *p; }

    /*  Prefix increment operator*/
    MyIterator& operator++() { ++p; return *this; }

    /*  Inequality comparison
    Allows the loop to check if two iterators point to different locations.
    Used in loop condition checks (it != end()).*/
    bool operator!=(const MyIterator& other) const { return p != other.p; }

private:
    int* p;
};

// A container with begin/end returning MyIterator
struct MyContainer {
    int data[3] = {100, 200, 300};

    MyIterator begin() { return MyIterator(&data[0]); }
    MyIterator end()   { return MyIterator(&data[3]); } // Returns an iterator pointing just past the last element (end marker for iteration).
};

void T1() {
    MyContainer c;

    // Deduction guide triggers: vector(It, It) -> vector<iterator_traits<It>::value_type>
    std::vector v(c.begin(), c.end());  // Deduces std::vector<int>

    for (int x : v)
        std::cout << x << " ";
}


/*  GOLE
Use a linked list to hold elements of a complex structure (e.g., Person).
Provide a custom iterator to enable range-based for loops over MyContainer.*/

// Complex structure
struct Person {
    int id;
    std::string name;

    Person(int id, const std::string& name) : id(id), name(name) {}
};

// Node for the linked list
struct Node {
    Person data;
    Node* next;

    Node(const Person& p) : data(p), next(nullptr) {}
};

// Custom forward iterator for linked list
class MyIteratorLL {
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type        = Person;
    using difference_type   = std::ptrdiff_t;
    using pointer           = Person*;
    using reference         = Person&;

    MyIteratorLL(Node* ptr) : current(ptr) {}

    reference operator*() const { return current->data; }
    MyIteratorLL& operator++() { current = current->next; return *this; }
    bool operator!=(const MyIteratorLL& other) const { return current != other.current; }

private:
    Node* current;
};

// Container holding a linked list
class MyContainerLL {
public:
    MyContainerLL() : head(nullptr), tail(nullptr) {}

    ~MyContainerLL() {
        while (head) {
            Node* tmp = head;
            head = head->next;
            delete tmp;
        }
    }

    void add(const Person& p) {
        Node* new_node = new Node(p);
        if (!tail) head = tail = new_node;
        else {
            tail->next = new_node;
            tail = new_node;
        }
    }

    MyIteratorLL begin() { return MyIteratorLL(head); }
    MyIteratorLL end() { return MyIteratorLL(nullptr); }

private:
    Node* head;
    Node* tail;
};




template <typename T>
struct MyVector {
    std::vector<T> data;

    MyVector(std::initializer_list<T> list) : data(list) {}

    void push_back(const T& val) {
        std::cout << "Inserting: " << val << '\n';
        data.push_back(val);
    }

    T& operator[](std::size_t i) {
        std::cout << "Accessing element at index " << i << '\n';
        return data[i];
    }

    void print() const {
        for (const auto& val : data)
            std::cout << val << " ";
        std::cout << '\n';
    }
};

// Deduction guide
template<typename T>
MyVector(std::initializer_list<T>) -> MyVector<T>;

int main() {
    MyVector v{10, 20, 30};  // MyVector<int>
    v.push_back(40);         // Logs insertion
    std::cout << v[2] << '\n'; // Logs access
    v.print();               // Prints contents
}
/*  ✅ Scenario: Adding Logging or Instrumentation to Vector Operations
Suppose you want every insert, erase, or access to be logged or validated for debugging, security, or profiling purposes. std::vector doesn’t allow you to override its behavior — so you need a wrapper like MyVector.

❌ Why std::vector can’t be used:
You can’t override or inject behavior like:

Logging

Range checking with custom messages

Access control

Profiling allocation or access times

std::vector isn’t designed for that. You’d have to:

Write a wrapper (MyVector)

Provide custom behavior

Possibly expose only a subset of the vector interface

✅ When you must use MyVector:
Logging or auditing all access/modification

Adding custom constraints on insertion/access

Overloading operators (e.g., [ ]) to change behavior

Interfacing with legacy/custom APIs expecting a non-standard container
*/



int main() {
    MyContainerLL people;
    people.add(Person(1, "Alice"));
    people.add(Person(2, "Bob"));
    people.add(Person(3, "Charlie"));

    for (const Person& p : people) {
        std::cout << p.id << ": " << p.name << '\n';
    }
}


/*      Perfect — here's a complete example where MyVector supports both:

construction from two iterators

construction from an initializer list

We’ll include both deduction guides so C++17 can deduce the type correctly.

✅ Full Example: MyVector with iterator & initializer list support*/

template<typename T>
struct MyVector {
    std::vector<T> data;

    // Constructor from iterator pair
    template<typename Iter>
    MyVector(Iter begin, Iter end) : data(begin, end) {
        std::cout << "Constructed MyVector from iterators\n";
    }

    // Constructor from initializer list
    MyVector(std::initializer_list<T> list) : data(list) {
        std::cout << "Constructed MyVector from initializer list\n";
    }

    void print() const {
        for (const auto& x : data)
            std::cout << x << ' ';
        std::cout << '\n';
    }
};

// Deduction guide for iterator constructor
template<typename Iter>
MyVector(Iter, Iter) -> MyVector<typename std::iterator_traits<Iter>::value_type>;

// Deduction guide for initializer list constructor
template<typename T>
MyVector(std::initializer_list<T>) -> MyVector<T>;

int main() {
    std::vector<int> base = {10, 20, 30, 40, 50};

    MyVector v1{1, 2, 3};                     // Uses initializer list
    MyVector v2(base.begin() + 1, base.end()); // Uses iterator constructor

    v1.print();
    v2.print();
}


// Would you like me to show how this integrates with std::vector using class template deduction next?
// Would you like a version using std::iterator traits or support for reverse iteration too?

// https://chatgpt.com/c/6811ee72-6f20-8000-94be-2bb2a9ae9f5b

