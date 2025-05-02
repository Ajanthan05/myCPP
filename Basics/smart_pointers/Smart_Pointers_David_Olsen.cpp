#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>

#include <memory>
#include <numeric> // for std::accumulate

using namespace std;

/*
What are Smart Pointers?
Smart pointers are template classes provided by the C++ Standard Library to manage the 
lifetime of dynamically allocated objects. They help prevent memory leaks and dangling 
pointers by automatically deleting the objects they point to when no longer needed.*/

/*  std::unique_ptr
Feature	Description
🎯 Exclusive ownership	One and only one unique_ptr can own the object
🔐 No memory leaks	Deletes object automatically when done
🛡️ Exception safe	Memory cleaned even if an exception is thrown
💨 Fast, lightweight	Just a pointer under the hood
Pros:
Lightweight and fast.
Prevents accidental sharing.
Cons:
No shared ownership (you must use std::shared_ptr for that).
Thread Safe Ref Count ❌
*/
std::unique_ptr<int> ptr1 = std::make_unique<int>(10);
// std::unique_ptr<int> b = a; // ❌ compile error
// Moving ownership
std::unique_ptr<int> ptr2 = std::move(ptr1); // ptr1 becomes nullptr

void UniquePtr() {
    /*  🧱 Custom Deleters
    You can use custom deleters — e.g., for arrays or special cleanup logic.
    */
    auto deleter = [](int* p) {
        cout << "Deleting Array...\n";
        delete[] p;
    };
    std::unique_ptr<int[], decltype(deleter)> arr(new int[10], deleter);

    /*  Managing Arrays:- Use unique_ptr<T[]> for arrays
    ⚠️ Use [], not *arr or .get()[0], for elements.*/
    std::unique_ptr<int[]> arr2(new int[5]);
    arr2[0] = 10;

    /*  When Not to Use unique_ptr
You want shared ownership → use std::shared_ptr

You don't need dynamic allocation at all → just use stack variables or containers

You're passing to legacy APIs needing raw pointers → use ptr.get()
*/

std::unique_ptr<int[]> arr(new int[5]);

    int* raw = arr.get();  // get the raw pointer

    // Modify through raw pointer
    raw[0] = 42;
    raw[1] = 99;

    /* It safe to edit through .get()   as long as the unique_ptr is still alive, the memory is valid. You’re just using another way to access the same data.
⚠️ Don't store the raw pointer and use it after the unique_ptr is destroyed — that would be a dangling pointer, which leads to undefined behavior.

.get()	Gives the raw pointer	❌ Still owned by unique_ptr
.release()	Gives the raw pointer and releases ownership	✅ You now own it and must delete it manually

*/
}

// Injecting ownership via constructor
class Resource {
public:
    Resource() { std::cout << "Resource acquired\n"; }
    ~Resource() { std::cout << "Resource released\n"; }

    void use() {
        std::cout << "Using resource\n";
    }
};
class Worker {
private:
    std::unique_ptr<Resource> res;

public:
    Worker(std::unique_ptr<Resource> r) : res(std::move(r)) {}

    void work() {
        res->use();
    }
};

void Injicting_Ownership() {
    auto r = std::make_unique<Resource>();
    Worker w(std::move(r));
    w.work();
}
    
// Managing a resource array    ✅ Automatically cleans up the buffer when AudioBuffer goes out of scope — no memory leaks.
class AudioBuffer {
private:
    std::unique_ptr<float[]> buffer;
    std::size_t size;

public:
    AudioBuffer(std::size_t samples)
        : buffer(std::make_unique<float[]>(samples)), size(samples) {}

    float& operator[](std::size_t index) {
        return buffer[index];
    }

    std::size_t length() const { return size; }
};

void AutomaticallyCleansUp() {
    AudioBuffer buf(1024);
    buf[0] = 1.0f;
    std::cout << "First sample: " << buf[0] << "\n";
}
    

void CustomDelete() {
    std::unique_ptr<int> ptr(new int[10]); // ❌ UB: uses delete, not delete[]
    // ✅ Correct way: use unique_ptr<T[]> with delete[]:
    std::unique_ptr<int[]> ptr(new int[10]); // ✔️ uses delete[]
    // But if you want extra logic (e.g., logging, cleanup, pooling), you need a custom deleter:
    auto logDeleter = [](int* p) {
        std::cout << "[LOG] Cleaning up buffer...\n";
        delete[] p;
    };
    
    std::unique_ptr<int[], decltype(logDeleter)> buffer(new int[256], logDeleter);
    /*  Use a custom deleter with arrays when:

You want to log, track, or wrap the cleanup.

You're managing non-memory resources: files, sockets, handles.

You're writing RAII wrappers for C libraries.

You're using smart pointer-based resource management in a framework (like Qt, SDL, Vulkan, etc.)
*/  

FILE* f = fopen("data.txt", "r");
auto fileDeleter = [](FILE* fp) {
    std::cout << "Closing file...\n";
    if (fp) fclose(fp);
};

std::unique_ptr<FILE, decltype(fileDeleter)> file(f, fileDeleter);

std::unique_ptr<FILE, decltype(&fclose)> file(fopen("data.txt", "r"), fclose);

/*  unique_ptr<T, Deleter>	Manages custom deletion logic
decltype(deleter)	Type-safe way to capture lambdas
Works great for arrays	Ensures delete[] is used
Good for non-memory cleanup	Like fclose, free(), or logging*/
}



using InputType = std::vector<float>;
using ResultType = float;

class HelperType {
public:
    explicit HelperType(const InputType& data) : data_(data) {}

    void calculate() {
        sum_ = std::accumulate(data_.begin(), data_.end(), 0.0f);
        std::cout << "Initial sum: " << sum_ << '\n';
    }

    void add_correction(float correction) {
        sum_ += correction;
        std::cout << "Applied correction: " << correction << '\n';
    }

    ResultType important_result() const {
        return sum_ / data_.size();  // average
    }

private:
    InputType data_;
    float sum_ = 0.0f;
};

// Free function that further modifies HelperType
void calculate_more(HelperType& helper) {
    helper.add_correction(1.5f); // simulate a correction factor
}

// The main function using unique_ptr
ResultType do_work(InputType inputs) {
    std::unique_ptr<HelperType> owner{new HelperType(inputs)};
    owner->calculate();
    calculate_more(*owner); // pass by reference
    return owner->important_result();
}

void RunHelperType() {
    InputType sensorData = {10.0f, 20.0f, 30.0f};
    ResultType result = do_work(sensorData);

    std::cout << "Final result: " << result << '\n';
}





std::shared_ptr<int> sp1 = std::make_shared<int>(20);
std::shared_ptr<int> sp2 = sp1; // reference count increases
// ✅ Pros:
// Great for shared resources.
// Easy to use with std::make_shared.

// ❌ Cons:
// Slightly more overhead (due to reference counting).
// Potential for cyclic references (use weak_ptr to break cycles).


/*  3. std::weak_ptr
A non-owning reference to an object managed by shared_ptr.

Used to observe or access shared objects without affecting their lifetime.

Can break cyclic dependencies in shared pointers. ???

✅ Pros:
Prevents memory leaks in shared cyclic references.
Safe way to access shared memory conditionally.
Use weak_ptr to handle observer patterns or prevent cycles.

❌ Cons:
Must always check validity using lock() before access.*/

void Weak_ptr() {
    std::shared_ptr<int> sp = std::make_shared<int>(30);
    std::weak_ptr<int> wp = sp;

    if (auto spt = wp.lock()) { // returns shared_ptr if object is still alive
        // use *spt
    }
}

//////////////////////////////////////////////////////////////////////
// classic RAII + std::unique_ptr

using InputType = std::vector<float>;
using ResultType = float;

class HelperType2 {
public:
    explicit HelperType2(const InputType& data) : data_(data) {}

    void calculate() {
        sum_ = std::accumulate(data_.begin(), data_.end(), 0.0f);
        std::cout << "Initial sum: " << sum_ << '\n';
    }

    void add_correction(float correction) {
        sum_ += correction;
        std::cout << "Applied correction: " << correction << '\n';
    }

    ResultType important_result() const {
        return sum_ / data_.size();  // average
    }

private:
    InputType data_;
    float sum_ = 0.0f;
};

// Free function that further modifies HelperType2
void calculate_more(HelperType2& helper) {
    helper.add_correction(1.5f); // simulate a correction factor
}

// The main function using unique_ptr
ResultType do_work(InputType inputs) {
    std::unique_ptr<HelperType2> owner{new HelperType2(inputs)};
    owner->calculate();
    calculate_more(*owner); // pass by reference
    return owner->important_result();
}

void classic_RAII() {
    InputType sensorData = {10.0f, 20.0f, 30.0f};
    ResultType result = do_work(sensorData);

    std::cout << "Final result: " << result << '\n';
}


/*
std::unique_ptr<T> createObject() {
    auto obj = std::make_unique<T>();
    // setup obj...
    return obj;  // moves out
}
// Behind the scenes, the compiler does an implicit move when returning a std::unique_ptr.

// Lazy initialization / deferred ownership:-  Imagine a class that initializes a resource only when needed:
class Manager {
    std::unique_ptr<T> resource;
public:
    void setup() {
        resource = std::make_unique<T>();
    }
};
Here, we move the newly created std::unique_ptr into the member variable.*/

// Template Manager class
template <typename T>
class Manager {
    std::unique_ptr<T> resource;
public:
    void setup() {
        resource = std::make_unique<T>();
    }

    void useResource() {
        if (resource) {
            resource->doSomething();
        } else {
            std::cout << "No resource available." << std::endl;
        }
    }
};

// Simple T class
class MyResource {
public:
    MyResource() {
        std::cout << "MyResource constructed!" << std::endl;
    }
    ~MyResource() {
        std::cout << "MyResource destructed!" << std::endl;
    }

    void doSomething() const {
        std::cout << "MyResource is doing something!" << std::endl;
    }
};
/*   Why use a template here?
Manager can now manage any type, not just MyResource.

Reusable for Texture, Sound, DatabaseConnection, Widget, etc.

Strong type-safety and zero-cost abstraction at compile time.*/
void Template_Manager_class() {
    Manager<MyResource> mgr;

    mgr.useResource();  // No resource yet

    mgr.setup();        // Creates MyResource
    mgr.useResource();  // Now resource can be used

    // When mgr goes out of scope, MyResource is deleted automatically
}


/*
void smart_Pointer_Ownership_Semantics() {
    auto a = std::make_unique<T>();
    // 'a' owns the object

    std::unique_ptr<T> b{ std::move(a) };
    // Now 'b' owns it, 'a' becomes empty (null)

    a = std::move(b);
    // Ownership moves back to 'a', and 'b' is empty now
    /*  With std::unique_ptr, only one pointer can own the resource at a time.
Moving is how you transfer ownership without copying (since unique_ptr is non-copyable).*/

    /*  storing dynamic objects inside a container: You can’t store raw std::unique_ptr in std::vector without moving*/
    /*
    std::vector<std::unique_ptr<T>> vec;
    auto obj = std::make_unique<T>();

    vec.push_back(std::move(obj)); // must move, cannot copy!
}
*/

/*  Transfer ownership
 To transfer ownership to a function, pass std::unique_ptr by value
    caller wont own the memory after making the call
 To return ownership from a function, return std::unique_ptr by value */
std::unique_ptr<float[]> science(std::unique_ptr<float[]> x, std::unique_ptr<float[]> y, int N) {
    auto z = std::make_unique<float[]>(N);
    // saxpy(2.5, x.get(), y.get(), z.get(), N);
    return z;
}

/*  Never pass a pointer to 2 different unique pointer object*/

/*  Two unique_ptr object own same memory: Crash due to double free*/
void test() {
auto p = make_unique<int>(new int);
unique_ptr<int> d{p.get()};
/* Dont create unique_ptr from row pointer */
}




/////////////////   DANGLING POINTER ////////////////////
/*  A dangling pointer is a pointer that still points to a memory location that is:
Freed, Out of scope, Invalid
The pointer is alive, but the memory it points to is dead.
Trying to use a dangling pointer = Undefined Behavior (crashes, corruptions, random behavior)*/

// Pointer to local (stack) variable
int* getPointer() {
    int x = 10;
    return &x;  // 🚨 Returning address of a local variable
}
// After getPointer() ends, x is destroyed!

// Double delete
int* p = new int(5);
// delete p;
// delete p;    // 🚨 Deleting same pointer again

// Object goes out of scope
// int* p;
// {
//     int x = 20;
//     p = &x;
// }
// x is destroyed here, but p still points to its old location 🚨

/*  🛡️ How to Prevent Dangling Pointers

✅ 1. Set pointers to nullptr after delete
Now, trying delete p; again is safe (deleting nullptr does nothing).
Accessing *p after setting to nullptr will cause an immediate crash in debug mode (better than silent corruption).

✅ 2. Don't return addresses of local variables
Instead, allocate dynamically or return by value.

✅ 2. Don't return addresses of local variables
Instead, allocate dynamically or return by value.

✅ 4. Scope-based resource management (RAII)
C++ style: let objects manage their lifetime automatically.

✅ 5. Be careful with pointer aliasing
If multiple pointers point to the same object, manage ownership clearly.

🚀 Bonus Tip
When debugging, compile your code with Address Sanitizer:

        g++ -fsanitize=address -g yourfile.cpp
It automatically catches most dangling pointer errors at runtime!
*/
// ❌ BAD CODE: Dangerous .get() usage (leads to dangling pointer)
struct Data {
    int value;
};

void useData(Data* ptr) {
    std::cout << "Value: " << ptr->value << std::endl;
}

void dangling_pointer() {
    Data* rawPtr = nullptr;
    {
        auto uptr = std::make_unique<Data>();
        uptr->value = 42;

        rawPtr = uptr.get(); // 🧨 Dangerous: saving raw pointer
        useData(rawPtr);     // OK here because uptr still alive
    } // uptr destroyed here. Memory freed.

    useData(rawPtr);  // 🚨 BOOM: rawPtr is dangling, undefined behavior!
}

/*  ✅ GOOD CODE: Fix 1 — Pass unique_ptr properly*/
// Best solution: pass ownership or a reference, NOT a raw pointer.
struct Data {
    int value;
};

void useData(const Data& ref) {  // Accept by reference
    std::cout << "Value: " << ref.value << std::endl;
}

int main() {
    auto uptr = std::make_unique<Data>();
    uptr->value = 42;

    useData(*uptr);  // Dereference safely while uptr is alive
} // uptr destroyed automatically here
// ✅ No raw pointer stored.
// ✅ No dangling.
// ✅ Memory safely cleaned up


/*  🧠 What is std::weak_ptr?
std::weak_ptr is a non-owning smart pointer.

It observes an object managed by a std::shared_ptr, without affecting its lifetime.

It doesn't increase the reference count like shared_ptr does.

Think of it like:

"I know where the object is — but I don't own it.
If it’s gone, I can check safely without crashing."

🎯 Why Do We Need weak_ptr?
Avoid circular references (e.g., two objects owning each other, causing memory leaks).

Safely check if an object still exists before accessing it.

Monitor resources without preventing them from being freed.*/
void sharedPtr() {
    std::weak_ptr<int> wp;

    {
        auto sp = std::make_shared<int>(42);
        wp = sp;  // weak_ptr observes sp

        if (auto temp = wp.lock()) {  // lock() returns shared_ptr
            std::cout << "Value: " << *temp << std::endl; // Safe access
        }
    } // sp is destroyed here

    if (auto temp = wp.lock()) {
        std::cout << "Still alive: " << *temp << std::endl;
    } else {
        std::cout << "Object already destroyed!" << std::endl;
    }
}

/*  🔥 Circular Reference Problem (without weak_ptr)
Imagine two classes holding shared_ptr to each other:

struct B; // forward declaration

struct A {
    std::shared_ptr<B> bptr;
};

struct B {
    std::shared_ptr<A> aptr;
};
A owns B, and B owns A.

Neither can be destroyed → memory leak! 🚨*/

// ✅ Solution: Break cycle with weak_ptr
struct B; // forward declaration

struct A {
    std::shared_ptr<B> bptr;
};

struct B {
    std::weak_ptr<A> aptr; // weak_ptr here!
};
/*  A owns B.
B just observes A.
No leak, memory freed properly!

When to Use weak_ptr

Prevent circular references	Break ownership cycles
Check if object still exists	lock() before accessing
Non-owning observers	Watch an object without keeping it alive  
  
shared_ptr<T> → owns T (reference count +1)

weak_ptr<T>   → watches T (reference count unchanged)
    └── lock() → returns shared_ptr if T still alive*/

    
int main() {


    return 0;
}