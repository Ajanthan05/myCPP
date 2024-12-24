#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

template <typename T>
class uniqueptr
{
private:
    T *res;
public:
    uniqueptr(T *a = nullptr) : res(a) {
        cout << "constructor\n";
    }

    uniqueptr(const uniqueptr<T>& ptr) = delete; // delete copy constructor
    uniqueptr& operator= (const uniqueptr<T>& ptr) = delete; // delete copy asignment constructor

    /* Move coppy consructor*/
    uniqueptr(uniqueptr<T>&& ptr) {   // && give RValue
        res = ptr.res;
        ptr.res = nullptr;   // Transwering he ownership
    }  

    /* Move coppy assignment operator*/
    // ptr3 = ptr2;
    uniqueptr& operator= (uniqueptr<T>&& ptr) {  
        if (this != &ptr) {
            if (res) {
                delete res;
            }
            res = ptr.res;
            ptr.res = nullptr;
        }
        
        return *this;
    }

    T* operator->() {
        return res;   // return row pointer
    }

    T& operator*() {
        return *res;
    }

    T* get() {
        return res;
    }

    void reset(T* newres = nullptr) {
        if(res) {
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
    // uniqueptr<int> ptr2(ptr1);   // compilation error
    // uniqueptr<int> ptr3 = ptr1;  // compilation error
    uniqueptr<int> ptr4(new int(500));
    // ptr4 = ptr3;                 // compilation error
    uniqueptr<int> ptr3 = std::move(ptr1);

    uniqueptr<int> ptr2(new int(2));
    ptr4 = std::move(ptr2);

    // ptr1->func();
    cout << *ptr4;
    ptr1.get();
    ptr1.reset(new int(30));

    cout << endl;


    return 0;
}