#include <iostream>
#include <vector>
#include <climits>
#include <memory>
#include <utility>
#include <algorithm>
#include <stack>

using namespace std;

class NaiveVector {
public:
    int *ptr;
    size_t size;
public:
    NaiveVector() : ptr{nullptr}, size{0} {}

    void push_back(int newValue) {
        int *nwePtr = new int[size + 1];
        std::copy(ptr, ptr + size, nwePtr);
        delete[] ptr;
        ptr = nwePtr;
        ptr[size++] = newValue;
    }
    // NaiveVector No longer leak memory on distruction
    ~NaiveVector() { delete[] ptr; }

    /*  Whenever u write a destructor, u probably need 
    to write a copy constructor  and copy assignmen operator as well.
    
    Copy constructor is responsible for duplicating 
    resources to avoid double frees*/
    NaiveVector(const NaiveVector& rhs) {
        size = rhs.size;
        ptr = new int[size];
        std::copy(rhs.ptr, rhs.ptr + size, ptr);
    }

    // COPY AND SWAP idiom
    // NaiveVector& operator=(const NaiveVector& rhs) {
    //     NaiveVector copy = rhs;
    //     copy.swap(*this);
    //     return *this;
    // }

    // NaiveVector& operator=(const NaiveVector& rhs) {
    //     delete[] ptr;
    //     ptr = new int[rhs.size];
    //     size = rhs.size;
    //     std::copy(rhs.ptr, rhs.ptr + size, ptr);
    //     return *this;
    // }
    /*  Not self move but still troublesome (for templated or recursive 
    data structor)*/

    // Rvalues wont't be missed
    NaiveVector(NaiveVector&& rhs) {
        ptr = std::exchange(rhs.ptr, nullptr);
        size = std::exchange(rhs.size, 0);
    }

    /*  Copy assignment operator free the left-hand resources and copy the right-hand one
        Move assignment operator free the left-hand resources and transfer ownership of the right-hand one
    */

};

void Test() {
    NaiveVector v;
    v.push_back(1);
    v.push_back(2);
    cout << *v.ptr << "\n";
    cout << *(v.ptr + 1)  << "\n";
    {
        NaiveVector w = v;
    }
    // cout << v[0] << "\n";
    cout << *v.ptr << "\n";

    {
        NaiveVector w;
        w = v;
    }
}

/*  Here 'rhs' is not reference to some heap allocated object
managed by a share_ptr 
if (rhs != this)  won't help*/
// struct A {
//     NaiveVector<shared_ptr<A>> m;
// };
// void T2() {
//     NaiveVector<shared_ptr<A>> v;
//     v = v[1]->m;
// }
/*  When an the exception is thrown, the runtime looks up
the call stack until it find a sutable catch handler for
the type of the exception being thrown. Assuming it finds one...

The Runtime performs stack unwinding. For every local scope 
between the throw and the catch handler, the runtime invokes the
distructorsof all local varibles in that scope.

To avoide leack, place all cleanup code in the distructors.
*/
struct RAIIPtr {
    int *ptr_;
    RAIIPtr(int *p) : ptr_(p) {}
    ~RAIIPtr() { delete[] ptr_; }

    // We can impprove RAIIPtr by making it non-copyable
    RAIIPtr(const RAIIPtr&) = delete;
    RAIIPtr& operator=(const RAIIPtr&) = delete;
};
void T_RAIIPtr() {
    try {
        RAIIPtr arr = new int[5];
        throw std::runtime_error("For Example");
    } catch (const std::exception& ex) {
        cout << ex.what() << "\n";
    }
}

void foo(const std::string& str) {
    // str[1] = 'A';        Read only
    cout << "LValue\n";    // Take Rvalue if below is not present
}
void foo(std::string&& str) {
    str[1] = 'A';
    cout << "RValue\n";    
}
void Reference() {
    std::string s = "Hello";
    foo(s);
    foo(s + " world");
}

class Vec {
public:
    int *ptr;
    size_t size;
public:
    Vec() : ptr{nullptr}, size{0} {}

    void push_back(int newValue) {
        int *nwePtr = new int[size + 1];
        std::copy(ptr, ptr + size, nwePtr);
        delete[] ptr;
        ptr = nwePtr;
        ptr[size++] = newValue;
    }    

    /*  Acopy constructor to copy the resources (Avoide double-frees )*/
    Vec(const Vec& rhs) {
        size = rhs.size;
        ptr = new int[size];
        std::copy(rhs.ptr, rhs.ptr + size, ptr);
    }

    /*  A move constructor to transfer ownership of the resource (cheaper than copying)*/
    Vec(Vec&& rhs) {
        ptr = std::exchange(rhs.ptr, nullptr);
        size = std::exchange(rhs.size, 0);
    }

    /*  A two-argument swap, to make your type efficiently "Std::swappable"*/
    friend void swap(Vec& a, Vec& b) noexcept {
        a.swap(b);
    }

    // A destructo, to free the resources (avoid leaks)
    ~Vec() { delete[] ptr; }

    // An assignment operator
    Vec& operator=(Vec copy) {
        copy.swap(*this);
        return *this;
    }

    void swap(Vec& rhs) noexcept {
        using std::swap;
        swap(ptr, rhs.ptr);
        swap(size, rhs.size);
    }
};

/*  unique_ptr is RAII class. 
It manages a raw pointer to a uniquely owned heap allocation. */
int main() {
    Test();

    return 0;
}