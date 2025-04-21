#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <utility>
#include <algorithm>
#include <string>
#include <map>
#include <set>

using namespace std;

/* Most of the standard containers and almost any use of smart poointers
(or new) uses the heap

Heap allocation are in arbitary location, so loacality of
reference is terrible between separate allocation
*/

// Passing vector by r-value reference
vector<int> load_number(vector<int>&& v) {
    for(int i=0; i<1000; ++i) {
        v.push_back(i);
    }
    // return v;   // copy constructor
    return move(v); // move connstructor
}

void rValue_reference() {
    vector<int> v;
    for(int n=0; n<9; ++n) {
        v.clear();
        v = load_number(move(v));
    }
}

// Passing vector by non_const reference
void load_number_non_const(vector<int>& v) 
{   // nothing      size0, capacity 1066
    for(int i=0; i<1000; ++i) {
        v.push_back(i); // 0 allocations
    }
}

void non_const_reference() 
{
    vector<int> v;
    for(int n=0; n<9; ++n) 
    {                               // size 1000, capacity 1066
        v.clear();                  // size 0   , capacity 1066
        load_number_non_const(v);   // nothing
    }
}

// Passing vector by r-value reference
// vector<int> load_number(vector<int>&& v) {
//     for(int i=0; i<1000; ++i) {
//         v.push_back(i);
//     }
//     // return v;   // copy constructor
//     return move(v); // move connstructor
// }

// void rValue_reference() {
//     vector<int> v;
//     for(int n=0; n<9; ++n) {
//         v.clear();
//         v = load_number(move(v));
//     }
// }

// Use shrink_to_fit() only if you need to reduce memory usage after you're done growing the vector.
/*
reserve() is a method, and methods must be called on a valid object, after 
it has been constructed. At global scope, only constant expressions 
(constexpr) or compile-time constant data is allowed during static initialization.
*/


/* Moving a string */
vector<string> v;
// v.reserve(100); ❌ ERROR: this is not allowed at global scope
/*
Global statements like v.reserve(100); are not inside a function or constructor.
You can’t call methods at the global level unless it's part of an initializer.
*/

void fun() {
    vector<string> v;
    v.reserve(100);

    string s = "Long string .....";
    v.push_back(move(s));      // here move doesn't help for small string

    v.emplace_back(move(s)); // doesntdo much wrong way to do it


    /* Optimal */
    auto& str = v.emplace_back("Hello");
    str += ", ";
    str += "World!";

    v.emplace_back("Hellow"); // it is faster than push_back
}

void Splicing_test() {
    /* splicing refers to transferring elements from one container to another without copying or moving them, just by relinking internal pointers
    ✅ It’s available in std::list (and std::forward_list) — not in vector, deque, or array.
    Because std::list is a doubly-linked list, and elements are connected by pointers. You can detach and reattach nodes in constant time.

    For std::vector, elements are contiguous in memory — no way to "splice" without copying or moving.
    
    void splice(iterator pos, list& other);
    void splice(iterator pos, list& other, iterator it);
    void splice(iterator pos, list& other, iterator first, iterator last);


    */

    std::list<int> a = {1, 2, 3};
    std::list<int> b = {10, 20, 30};

    // Move all elements from b into a at position a.begin()
    a.splice(a.begin(), b);
    // a = {10, 20, 30, 1, 2, 3}
    // b = {} (emptied)
    // ✅ No allocations, no copies — just pointer adjustment.

    // 2: Move a single element
    std::list<int> a1 = {1 , 2, 3};
    std::list<int> b1 = {10, 20, 30, 40, 50, 60};

    // auto it = std::next(b1.begin()+1); For bidirectional or forward iterators, always prefer std::next() or std::advance()
    auto it = std::next(b1.begin(), 2);
    // or
    auto it2 = b1.begin();
    std::advance(it2, 2); // No return type
    // Now it2 also points to 30

    a1.splice(a1.end(), b1, it);

    for(auto &i : a1) cout << i << " ";
    cout << "\n";

    // 3: Move a range
    
    auto it3 = b1.begin();
    std::advance(it2, 2);       // O(2)

    auto end = it3;
    std::advance(end, 2);       // O(2) — because end = it2 + 2 steps

    std::advance(end, 4);
    a.splice(a.begin(), b1, it3, end);

    for(auto &i : a) cout << i << " ";
    cout << "\n";
}

void Node_extraction() {
    /* node extraction in C++ (specifically from associative containers 
    like std::map, std::set, std::unordered_map, etc.)
   
    Extract a key-value pair from a container without destroying it.

    Modify the key or value outside the container.

    Reinsert it into the same or a different container.

    Do all this without copying/moving the data — just like "plucking" a node out of a tree or hash map.
    
    auto node = my_map.extract(key);

    node is now a std::map::node_type object.

If key doesn't exist → node.empty() == true.

other_map.insert(std::move(node));

✅ When to Use It

Use Case	Benefits
Modifying keys in a map	Keys are normally immutable inside a map
Moving elements between containers	Efficient — no copy/move of stored values
Conditional re-insertion	You can hold and decide later where to insert

    */

    // 1. Modify Key in Map
    std::map<int, std::string> m = {
        {1, "one"},
        {2, "two"}
    };
    
    // Extract and change key
    auto node = m.extract(2);     // removes {2, "two"}
    node.key() = 42;              // modify key
    m.insert(std::move(node));    // insert back as {42, "two"}
    
    // 2. Moving Between Maps
    std::map<int, std::string> src = {{1, "A"}, {2, "B"}};
    std::map<int, std::string> dst;

    for (auto it = src.begin(); it != src.end(); ) {
        auto node = src.extract(it++);
        dst.insert(std::move(node));
    }

    /* Available Since  C++17
    Containers: std::map, std::multimap, std::set, std::unordered_map, 
    std::unordered_set, etc.
    */

    // Accessing the Node
    // auto node = my_map.extract(key);
    auto node1 = m.extract(1);
    if (!node1.empty()) {
        std::cout << "Key: " << node1.key() << "\n";
        std::cout << "Value: " << node1.mapped() << "\n"; // for map
    }
}
 
void Merging_Sets() {
    set<int> src = {1,3,5};
    set<int> dst = {2,4,5};

    dst.merge(src);
    // src == {5}  common element
    // dst == {1,2,3,4,5}
}

/////////////// Efficient_factories /////////////////
auto new_record(const char* str) {
    static int id = 0;
    map<int, string> tmp;
    tmp.emplace(++id, str);
    return tmp.extract(tmp.begin());
}
void Efficient_factories() {
    

    map<int, string> table;
    table.insert(new_record("Ajanthan"));
    table.insert(new_record("Arun"));

    for(auto &it : table) {
        cout << it.first << ": " << it.second << "\n";
    }
    cout << "\n";
}


/////////////// Add Case studey
struct foo {
    template<typename STR>
    foo(int i, double d, char c, STR&& s) 
    : i(i), d(d), c(c), s(forward<STR>(s)) {}

/*The const char* s is passed to the foo constructor as a const char*, and thanks to template deduction, STR = const char*.
The std::string constructor that takes a const char* is implicitly used, constructing the string in-place.
*/
    int i;
    double d;
    char c;
    string s;
};

class bar {
    vector<foo> foos;
public:
    void add(int i, double d, char c, const char* s) {
        foos.emplace_back(i, d, c, s); // emplace_back constructs the foo object in-place in the vector, avoiding an extra copy or move.
    }
};

class bar_O {
    vector<foo> foos;
public:
    template<typename... T>
    void add(T&&... t) {
        foos.emplace_back(std::forward<T>(t)...); // emplace_back constructs the foo object in-place in the vector, avoiding an extra copy or move.
    }
};

void All_move() {
    bar_O b;
    // 1. const char*
    const char* cstr = "const char* string";
    b.add(1, 1.1, 'A', cstr);

    // 2. string literal (which decays to const char*)
    b.add(2, 2.2, 'B', "string literal");

    // 3. std::string (lvalue)
    std::string str = "std::string lvalue";
    b.add(3, 3.3, 'C', str);

    // 4. std::string (rvalue)
    b.add(4, 4.4, 'D', std::string("std::string rvalue"));
}
void Efficient_move() {
    bar_O b;
    std::string name = "hello";
    b.add(1, 2.0, 'x', std::move(name)); // Efficient move

}

////////////// Container Choice ////
/* When shoulden't I Use vectors
Vector is the go-to for dynamic storage
contiguous memory
Fastest possible traversal
random access
Growth invalidates everything if it grows
    -When you know the final size(even roughly) pre-reserve
    -Pre-reserve to avoid thrashing( if you can affort it)
    -U can shrink to fit later (but it may not help)
Geometric growth behavior makes large vectors impossible
unless the max size is known ahead of time
*/

/* ARRAY
Contiguous memory
Fastest possible traversal
Random access
Fixedsize
Local object (stack or embedded)
*/

/*
invalidation" refers to situations where certain operations on a container make previously valid references, iterators, or pointers invalid — meaning they now point to unpredictable or wrong memory, and using them leads to undefined behavior.
*/
int main() {
    Splicing_test();
    Node_extraction();

    Efficient_factories();
    return 0;
}