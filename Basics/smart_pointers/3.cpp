#include <iostream>
#include <vector>
#include <climits>

using namespace std;

// RAII : Resource Acquisition In Initialisation

template <typename T>
class uniqueptr {
private:
    T *res;
public:
    uniqueptr(T *a = nullptr) : res(a) {
        cout << "constructor\n";
    }
    // Delete copy constructor
    uniqueptr(const uniqueptr<T> &ptr) = delete;

    // Delete copy assignment operator ptr4 = ptr3;   
    uniqueptr& operator=(const uniqueptr<T> &ptr) = delete;

    // move copy constructor
    uniqueptr(uniqueptr<T>&& ptr) {
        res = ptr.res;
        ptr.res = nullptr;
    }

    // move assignment operator
    uniqueptr& operator= (uniqueptr<T>&& ptr) {
        if (this != &ptr) {
            if (this->res) {
                delete res;
            }
            res = ptr.res;
            ptr.res = nullptr;
        }
        return *this;
    }

    /* Operator Overloading ->
    Here we are returning row pointer
    */
    T* operator->() {
        return res;
    }

    T& operator*() {
        return *res;
    }

    // ptr1.get();
    T* get() {
        return res;
    }

    // ptr1.reset(new int(30));
    void reset(T* newres = nullptr) {
        if (res) {
            delete res;
        }
        res = newres;
    }

    ~uniqueptr() {
        if (res) {
            delete res;
            res = nullptr;
        }
    }
};

int main() {
    uniqueptr<int> ptr1(new int(2));
    // uniqueptr<int> ptr2(ptr1);  // compilation error
    // uniqueptr<int> ptr2 = ptr1;    // compilation error
    uniqueptr<int> ptr4(new int(500));
    // ptr4 = ptr3;            // compilation error
    uniqueptr<int> ptr3 = std::move(ptr1);
    ptr4 = std::move(ptr1);

    // ptr1->func();
    cout << *(ptr3) << "\n";
    ptr1.get();
    // ptr1.reset(new int(30));

    return 0;
}