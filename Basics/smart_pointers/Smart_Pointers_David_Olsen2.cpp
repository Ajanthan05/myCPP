#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>

#include <memory>
#include <utility> // for std::exchange
using namespace std;



// template <typename T> struct Unique_ptr {
//     // ...
//     Unique_ptr(Unique_ptr const&) = delete;
//     Unique_ptr(Unique_ptr&& o) noexcept: ptr(std::exchange(o.ptr, nullptr)) { }

//     Unique_ptr& operator=(Unique_ptr const&) = delete;

//     Unique_ptr& operator=(Unique_ptr&& o) noexcept {
//         delete ptr;
//         ptr = o.ptr;
//         o.ptr = nullptr;
//         return *this;
//     }

//     T& operator*() const noexcept {
//         return *ptr;
//     }

//     T* operator->() const noexcept {
//         return ptr;
//     }

//     T* release() noexcept {
//         T* old = ptr;
//         ptr = nullptr;
//         return old;
//     }

//     void reset(T* p = nullptr) noexcept {
//         delete ptr;
//         ptr = p;
//     }

//     T* get() const noexcept {
//         return ptr;
//     }

//     explicit operator bool() const noexcept {
//         return ptr != nullptr;
//     }
// };



template <typename T>
class Unique_ptr1 {
    T* ptr;

public:
    // Default constructor
    Unique_ptr1() noexcept 
        : ptr(nullptr) { }

    // Constructor from raw pointer
    explicit Unique_ptr1(T* p) noexcept 
        : ptr(p) { }

    // Destructor
    ~Unique_ptr1() noexcept {
        delete ptr;
    }

    // Delete copy constructor
    Unique_ptr1(const Unique_ptr1&) = delete;

    // Move constructor
    Unique_ptr1(Unique_ptr1&& other) noexcept 
        : ptr(std::exchange(other.ptr, nullptr)) { }

    // Delete copy assignment
    Unique_ptr1& operator=(const Unique_ptr1&) = delete;

    // Move assignment
    Unique_ptr1& operator=(Unique_ptr1&& other) noexcept {
        if (this != &other) {
            delete ptr;
            ptr = std::exchange(other.ptr, nullptr);
        }
        return *this;
    }

    // Dereference operator
    T& operator*() const noexcept {
        return *ptr;
    }

    // Arrow operator
    T* operator->() const noexcept {
        return ptr;
    }

    // Release ownership of the pointer
    T* release() noexcept {
        T* old = ptr;
        ptr = nullptr;
        return old;
    }

    // Reset with a new pointer (deletes the old one)
    void reset(T* p = nullptr) noexcept {
        if (ptr != p) {
            delete ptr;
            ptr = p;
        }
    }

    // Get the raw pointer
    T* get() const noexcept {
        return ptr;
    }

    // Conversion to bool
    explicit operator bool() const noexcept {
        return ptr != nullptr;
    }
};

// template <typename T, typename... Args>
// Unique_ptr1<T> make_unique(Args&&... args);



struct MyClasS {
    MyClasS(int a, std::string b) : x(a), y(b) {
        std::cout << "MyClasS constructed with: " << x << ", " << y << std::endl;
    }

    int x;
    std::string y;
};

// template <typename T, typename... Args>
// unique_ptr<T> make_unique(Args&&... args) {
//     // Use placement new to construct the object and return a unique_ptr
//     return unique_ptr<T>(new T(std::forward<Args>(args)...));
// }

/*  Shared_ptr
use_count is not relaiable in multi threaded exvironment:- value
of the counter might change before u use the value that was returned
Shouldent be used in producdution code in multi-threaded environment

make share:- One memory block for the object and the control block

 To share ownership, additional shared_ptr objects must be created or assigned from 
an existing shared_ptr, not from the raw pointer
 {
 T* p = ...;
 std::shared_ptr<T> a(p);
 std::shared_ptr<T> b(p);
 } // runtime error: double freef
*/

void T() {
    // Create a unique_ptr to MyClasS using make_unique
    auto ptr = make_unique<MyClasS>(10, "Hello");

    std::cout << "x = " << ptr->x << ", y = " << ptr->y << std::endl;
}




/*  std::make_shared<T[]>(size) only works if T is default-constructible.  
make_shared<Object[]>(size) won’t work if Object has no default constructor

Solution: Manual Allocation with Custom Deleter
1. Allocate with new
2. Use std::shared_ptr with a custom deleter to ensure proper delete[] cleanup*/

struct Object {
    int id;
    Object(int i) : id(i) {
        std::cout << "Constructed Object " << id << "\n";
    }
    ~Object() {
        std::cout << "Destructed Object " << id << "\n";
    }
};

void Test() {
    // Allocate an array manually
    Object* rawArr = new Object[3]{
        Object(1),
        Object(2),
        Object(3)
    };

    // Create a shared_ptr with custom deleter for arrays
    std::shared_ptr<Object> sharedArr(rawArr, [](Object* p) {
            delete[] p;  // Correctly deletes the whole array
        }
    );
/*  Access Caveat
Because shared_ptr<Object> manages the first element’s pointer (rawArr), you can access the array using .get() and pointer arithmetic:*/
    std::cout << "Accessing shared array elements:\n";
    for (int i = 0; i < 3; ++i) {
        std::cout << sharedArr.get()[i].id << "\n";
    }

    // sharedArr goes out of scope => delete[] rawArr
}

/*  Alternative: Use std::vector of shared_ptr<Object>
Another safe and idiomatic way in modern C++*/

void ObjArray() {
    std::vector<std::shared_ptr<Object>> vec;
    vec.emplace_back(std::make_shared<Object>(1));
    vec.emplace_back(std::make_shared<Object>(2));
    vec.emplace_back(std::make_shared<Object>(3));

    // Access:
    for (const auto& obj : vec) {
        std::cout << obj->id << "\n";
    }
}
// ✅ Each object is managed separately
// ✅ Clean and readable
// ✅ No custom deleter needed






/*  expired()     	Check if the resource weak_ptr pointing to exists or not.
lock()	If the resource pointed by weak_ptr exists, this function returns a shared_ptr with
 ownership of that resource. If the resource does not exist, it returns default constructed shared_ptr.
use_count()	Tells about how many shared_ptr owns the resource.*/


struct Child;  // forward declaration
struct Parent {
    std::shared_ptr<Child> child;
    ~Parent() { std::cout << "Parent destroyed\n"; }
};

struct Child {
    std::weak_ptr<Parent> parent;  //  weak_ptr avoids cycle
    ~Child() { std::cout << "Child destroyed\n"; }
};

void WeakPtr() {
    auto p = std::make_shared<Parent>();
    auto c = std::make_shared<Child>();

    p->child = c;
    c->parent = p;

    // Both can be destroyed properly
}

/*  🔷 Why do we need these references?
✅ They’re natural for:

Navigation (parent ↔ child)
Bidirectional communication (subject ↔ observer)
Data structures (graph nodes)

✅ They enable intuitive, object-oriented designs.

🔴 But why do we need weak_ptr?
weak_ptr breaks ownership cycles:

Cyclic references: the lifetime of A and B depends on each other.

Using weak_ptr for the back-reference (like Child → Parent) ensures only one side owns the other.*/

/*  🟩 Example: GUI Widget Hierarchy
✅ Children own their children (shared_ptr)
✅ Parent is a weak observer*/
struct Widget : std::enable_shared_from_this<Widget> {
    std::vector<std::shared_ptr<Widget>> children;
    std::weak_ptr<Widget> parent;  // ✅ weak_ptr to avoid cycle
};

/*  What is enable_shared_from_this?
1. It’s a helper base class in the C++ Standard Library.
2. It enables an object to safely create a shared_ptr to itself.

std::enable_shared_from_this<T>, it provides:
🔹 A safe way to create a shared_ptr to the same control block as the one managing your object.*/
struct MyClass3 : std::enable_shared_from_this<MyClass3> {
    void printSelf() {
        // ✅ Create shared_ptr to this
        auto self = shared_from_this();
        std::cout << "Shared count: " << self.use_count() << "\n";
    }
};

void T_enable_shared_from_this() {
    auto p = std::make_shared<MyClass3>();
    p->printSelf();  // prints 2 because p + self both share ownership
}





struct Node : std::enable_shared_from_this<Node> {
    std::string name;
    std::vector<std::weak_ptr<Node>> neighbors;  // use weak_ptr to avoid cycles!

    Node(const std::string& n) : name(n) {
        std::cout << "Node " << name << " created\n";
    }

    ~Node() {
        std::cout << "Node " << name << " destroyed\n";
    }

    void connect(const std::shared_ptr<Node>& other) {
        neighbors.push_back(other);
        other->neighbors.push_back(shared_from_this());
    }

    void printNeighbors() {
        std::cout << "Neighbors of " << name << ": ";
        for (const auto& w : neighbors) {
            if (auto spt = w.lock()) {
                std::cout << spt->name << " ";
            }
        }
        std::cout << "\n";
    }
};

void Test_shared_from_this() {
    auto n1 = std::make_shared<Node>("A");
    auto n2 = std::make_shared<Node>("B");

    n1->connect(n2);  // bidirectional!

    n1->printNeighbors();
    n2->printNeighbors();

    std::cout << "Exiting scope...\n";
}

struct Widget2 : std::enable_shared_from_this<Widget2> {
    std::string name;
    std::vector<std::shared_ptr<Widget2>> children;
    std::weak_ptr<Widget2> parent;  // ✅ weak_ptr breaks cycle

    Widget2(const std::string& n) : name(n) {
        std::cout << "Widget2 " << name << " created\n";
    }

    ~Widget2() {
        std::cout << "Widget2 " << name << " destroyed\n";
    }

    void addChild(const std::shared_ptr<Widget2>& child) {
        children.push_back(child);
        child->parent = shared_from_this();
    }
};
/*  Key Differences
1️⃣ Hierarchy vs Graph

Widget is a tree-like hierarchy: parent → children

Node is a graph: nodes can form cycles (A ↔ B ↔ A)

2️⃣ Back-reference management

In Widget, only the parent pointer uses weak_ptr.

In Node, both sides of the neighbor relationship use weak_ptr because graphs naturally have cycles.

3️⃣ Semantics

Widget: one parent, multiple children → natural hierarchy.

Node: multiple neighbors (no explicit parent), mutual references.*/


/*  C++17 emphasizes the use of owner_before() for ordering weak/shared pointers, especially when used in associative containers like std::map or std::set.
std::weak_ptr<int> wp1, wp2;
if (wp1.owner_before(wp2)) {
    // wp1 is ordered before wp2
}
This doesn't compare the values pointed to, but the ownership (control block).

*/


/*  std::owner_less Improvements
C++17 improves how std::owner_less works with shared_ptr and weak_ptr. It can now be used more reliably in ordered containers like std::map and std::set, supporting mixed comparisons.

std::map<std::shared_ptr<int>, std::string, std::owner_less<std::shared_ptr<int>>> sharedMap;
std::map<std::weak_ptr<int>, std::string, std::owner_less<std::weak_ptr<int>>> weakMap;*/


/*  Behavior clarified: You must still not call shared_from_this() in constructors unless the object is already in a 
shared_ptr. But std::make_shared is now compatible with enable_shared_from_this more reliably.*/

/*  ✅ 3. owner_before() for Ordering Pointers
Used to compare ownership (control block), not values.

std::shared_ptr<int> sp1(new int(10));
std::shared_ptr<int> sp2(new int(20));

bool result = sp1.owner_before(sp2);  // true or false: based on internal control block
Use Case: When inserting smart pointers into containers like std::map, where sorting is required.

✅ 4. owner_less for Mixed Containers
Allows mixing shared_ptr and weak_ptr in the same container:

std::set<std::shared_ptr<int>, std::owner_less<>> ptrSet;

std::shared_ptr<int> sp1 = std::make_shared<int>(100);
std::weak_ptr<int> wp1 = sp1;

ptrSet.insert(sp1);
ptrSet.insert(wp1);  // ✅ OK in C++17
Why it's useful: You can use both strong and weak references in the same container while maintaining correct ordering.*/



/*  owner_before() is a comparison method for smart pointers (shared_ptr, weak_ptr) that:

✅ Does not compare the pointed-to objects
✅ Compares ownership of the underlying control block

It’s like saying:
“Does my control block come before yours?”
rather than:
“Does my object’s value come before yours?”*/



/*  if you are inside the object itself and want to hand out a shared_ptr to 
yourself, how do you do it?
struct MyClass {
    std::shared_ptr<MyClass> getPtr() {
        return std::shared_ptr<MyClass>(this); // ❌ BAD
    }
};
This creates a new shared_ptr with its own reference count, separate from the original shared_ptr.
➡️ Result: double-delete when both go out of scope (undefined behavior).
*/
struct MyClass : enable_shared_from_this<MyClass> {
    shared_ptr<MyClass> getPtr() {
        return shared_from_this(); // safe
    }
    ~MyClass() { cout << "Destroyed\n"; }
};
void SharedFromThis() {
    auto p1 = make_shared<MyClass>();
    auto p2 = p1->getPtr(); // another shared_ptr pointing to same object

    cout << "use_count = " << p1.use_count() << "\n"; // 2
}
struct NodE : std::enable_shared_from_this<NodE> {
    std::string name;
    std::weak_ptr<NodE> parent;
    std::vector<std::shared_ptr<NodE>> children;

    NodE(std::string n) : name(std::move(n)) {}
    ~NodE() { std::cout << name << " destroyed\n"; }

    void addChild(std::shared_ptr<NodE> child) {
        child->parent = shared_from_this(); // ✅ works
        children.push_back(child);
    }

    void showParent() {
        if (auto p = parent.lock())
            std::cout << name << " has parent: " << p->name << "\n";
        else
            std::cout << name << " has no parent\n";
    }
};

int Family_Tree() {
    auto root = std::make_shared<NodE>("Root");
    auto child1 = std::make_shared<NodE>("Child1");
    auto child2 = std::make_shared<NodE>("Child2");

    root->addChild(child1);
    root->addChild(child2);

    child1->showParent();
    child2->showParent();

    return 0; // ✅ avoid warning
}
/*  How does it work?

enable_shared_from_this<T> contains a hidden weak_ptr<T> inside your object.

When you create the object with make_shared, that weak_ptr is initialized to point to the same control block.

shared_from_this() simply calls weak_ptr.lock() and returns a new shared_ptr sharing ownership.*/


/*  template<typename T>
using Vec = std::vector<T>;

Vec<int> v = {1, 2, 3};     // std::vector<int>
Vec<double> w = {1.1, 2.2}; // std::vector<double>

2. Using-declaration (import names into scope)

This is about scope.
It lets you pull in a name from another namespace or base class.

Example (namespace scope):
#include <iostream>
using std::cout;
using std::endl;


Now you can use cout and endl without std::.

⚠️ Compare with:

using namespace std; // imports *everything* (dangerous in headers)


So using std::cout; (specific) is safer.

struct Base {
    void show(int) {}
};

struct Derived : Base {
    using Base::show; // bring Base::show into scope
    void show(double) {}
};
Without using Base::show;, the int overload would be hidden by the double version.


*/

struct Container {
    using VecInt = std::vector<int>;  // alias type
    VecInt data;                      // actual member variable
};
void TUsing() {
    Container it;
    it.data = {1, 2, 3};  // assign values

    // print
    for (int x : it.data) {
        cout << x << " ";
    }
    cout << "\n";

    // Alternative: no data, just local If you just want to use the alias 
    // inside a function
    Container::VecInt v = {1, 9, 3};  // use alias

    for (int x : v) {
        cout << x << " ";
    }
    cout << "\n";
}



int main() {
    // WeakPtr();
    // Test_shared_from_this();

    Family_Tree();

    TUsing();
    return 0;
}