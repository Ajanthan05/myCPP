#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <algorithm>
#include <stack>

#include <utility> // for std::exchange
using namespace std;



template <typename T> struct unique_ptr {
    // ...
    unique_ptr(unique_ptr const&) = delete;
    unique_ptr(unique_ptr&& o) noexcept: ptr(std::exchange(o.ptr, nullptr)) { }

    unique_ptr& operator=(unique_ptr const&) = delete;

    unique_ptr& operator=(unique_ptr&& o) noexcept {
        delete ptr;
        ptr = o.ptr;
        o.ptr = nullptr;
        return *this;
    }

    T& operator*() const noexcept {
        return *ptr;
    }

    T* operator->() const noexcept {
        return ptr;
    }

    T* release() noexcept {
        T* old = ptr;
        ptr = nullptr;
        return old;
    }

    void reset(T* p = nullptr) noexcept {
        delete ptr;
        ptr = p;
    }

    T* get() const noexcept {
        return ptr;
    }

    explicit operator bool() const noexcept {
        return ptr != nullptr;
    }
};



template <typename T>
class unique_ptr {
    T* ptr;

public:
    // Default constructor
    unique_ptr() noexcept 
        : ptr(nullptr) { }

    // Constructor from raw pointer
    explicit unique_ptr(T* p) noexcept 
        : ptr(p) { }

    // Destructor
    ~unique_ptr() noexcept {
        delete ptr;
    }

    // Delete copy constructor
    unique_ptr(const unique_ptr&) = delete;

    // Move constructor
    unique_ptr(unique_ptr&& other) noexcept 
        : ptr(std::exchange(other.ptr, nullptr)) { }

    // Delete copy assignment
    unique_ptr& operator=(const unique_ptr&) = delete;

    // Move assignment
    unique_ptr& operator=(unique_ptr&& other) noexcept {
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
// unique_ptr<T> make_unique(Args&&... args);



struct MyClass {
    MyClass(int a, std::string b) : x(a), y(b) {
        std::cout << "MyClass constructed with: " << x << ", " << y << std::endl;
    }

    int x;
    std::string y;
};

template <typename T, typename... Args>
unique_ptr<T> make_unique(Args&&... args) {
    // Use placement new to construct the object and return a unique_ptr
    return unique_ptr<T>(new T(std::forward<Args>(args)...));
}

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
int main() {
    // Create a unique_ptr to MyClass using make_unique
    auto ptr = make_unique<MyClass>(10, "Hello");

    std::cout << "x = " << ptr->x << ", y = " << ptr->y << std::endl;

    return 0;
}
int main() {


    return 0;
}