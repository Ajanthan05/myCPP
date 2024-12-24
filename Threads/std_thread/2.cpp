#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>
using namespace std;

// NOTE
// If we create multiple threads at the same time it doesn't guarantee which one will start first.

// 1. Function Pointer
void fun(int x) {
    while (x --> 0) {
        cout << x << endl;
    }
}

// 3.Functor (Function Object)
class Base {
public:
    void operator ()(int x) {
        while(x-- > 0) {
            cout << x << endl;
        }
    }
};

// 4. Non-static member function
class Base4 {
public:
    void run(int x) {
        while(x-- > 0) {
            cout << x << endl;
        }
    }
};

// 5. Static member function
class Base5 {
public:
    static void run(int x) {
        while(x-- > 0) {
            cout << x << endl;
        }
    }
};

int main() {
    std::thread t1(fun, 11);
    t1.join();

    // 2. Lambda Function
    
    auto fun = [](int x) {
        while (x --> 0) {
            cout << x << endl;
        }
    };

    std::thread t2(fun, 10);
    t2.join();

    cout << endl;
    // We can directly inject lambda at thread creaion time.
    std::thread t2_([](int x) {
        while (x --> 0) {
            cout << x << endl;
        }
    }, 10);
    t2_.join();

// 3.Functor (Function Object)
    std::thread t3( (Base()), 10);
    t3.join();

    cout << endl;

// 4. Non-static member function
    Base4 b4;
    std::thread t4(&Base4::run, &b4, 10);
    t4.join();

    cout << endl;

// 5. Static member function
// No need an object to call a function
    std::thread t5(&Base5::run, 10);
    t5.join();

    return 0;
}