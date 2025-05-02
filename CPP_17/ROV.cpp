#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>

using namespace std;

/* Return Value Optimization (ROV) in C++ ia a compiler optimization that eleminates
the creation of temporary object by directly constructiong the
return value in the memory location of the caller. 

This avoids the overhead of:

Copy constructor

Move constructor

Turn off RVO  -fno-elide-constructors
*/

class MyObj {
public:
    MyObj() = default;
    int a;
    MyObj(int x) {
        cout << "Constructor\n";
    }
    MyObj(const MyObj &obj) { // copy constructor
        cout << "Copy Constructor\n";
        a = obj.a;
    }

    // MyObj(MyObj &&obj) : a{ obj.a } { // copy constructor
    //     cout << "Move Constructor\n";
    //     obj.a = nullptr;
    // }
};

class MyObj2 {
public:
    int *a;
    MyObj2(int x) {
        cout << "Constructor\n";
        a = new int;
        *a = x;
    }
    MyObj2(const MyObj2 &obj) { // copy constructor
        cout << "Copy Constructor\n";
        a = obj.a;
        // Deep copy
    }

    MyObj2(MyObj2 &&obj) : a{ obj.a } { // copy constructor
        cout << "Move Constructor\n";
        obj.a = nullptr;
    }
};

/* Object construction happen outside the scope of the current function
*/

// When the return type is not the same as what is being returned 
class MyObjChild : public MyObj {
public:
    MyObjChild() {

    }
};
MyObj inheritanceFailsROV() {
    return MyObjChild();
}


/*
NROV: Named ROV
(U)ROV: Unnamed ROV  returning PR value directly
Compilers are required to provide URVO SUPPORT SINCE C++17
NRVO support is optional, bot recomanded
*/

/*  When there are multipple return statements returning 
different objects (for NRVO only)

Compile time compiler dont know which branch going to hit
*/
MyObj example2() {
    /* 2 possible return values of the same type */
    MyObj x1 = MyObj(3);
    MyObj x2 = MyObj(3);
    int a = rand() % 100;
    if(a > 5) return x1;
    return x2;
}

/*  When ur returning a complex expression 
(for NRVO only)*/
MyObj example_contd() {
    MyObj x1 = MyObj(3);
    MyObj x2 = MyObj(3);
    int a = rand() % 100;
    return std::move(a > 50 ? x1 : x2);
}


/*  For not garanteed ROV(e.g. NRVO): Either move or copy is sufficient 
/*  For     garanteed ROV(e.g. NRVO): No     move or copy is sufficient */
class MyObj4 {
public:
    // MyObj4() = default;
    MyObj4(int x) {
        cout << "Constructor\n";
    }
    MyObj4(const MyObj4 &obj) = delete;
    MyObj4(MyObj4 &&obj) = delete;
};
    
MyObj4 example4() {
    MyObj4 a = MyObj4(3);
    // return a; Doesn't compile
    return MyObj4(3); // If u return PR value it will compile
}

MyObj example5() {
    MyObj a = MyObj(3);
    // It's complex operation. its not not venilla L value or PR value
    return std::move(a);
    // std::move(a), which is an xvalue (expiring value)—not a prvalue (pure rvalue)
}

/* CV qua;lifiers don't actually affect ROV*/
const MyObj example6() {
    MyObj a1 = MyObj(3);
    return a1;
}


MyObj example7() {
    // MyObj x1 = MyObj(3);
    // MyObj x2 = MyObj(5);
    int a = rand() % 100;
    // return (a > 50 ? x1 : x2);
    return (a > 50 ? MyObj(3) : MyObj(5));
}

/* ROV  Compoler know what is return*/
MyObj2 example8() {
    /* 2 possible return values of the same type */
    MyObj2 x = MyObj2(3);
    int a = rand() % 100;
    if(a > 5) {
        *(x.a) = a;
        return x;
    } else {
        *(x.a) = 7;
        return x;
    }
}

/* No ROV */
class MyObj9 {
public:
    MyObj2 b;

    MyObj9() = default;
};

// MyObj2 example9() {
//     MyObj9 x = MyObj9(4);
//     return x.b;
// }

/* No ROV:- Volatile tells the compiler that the value of the variable may 
change at any time, without any action being*/
class MyObj10 {
public:
    int *a;
    MyObj10(int x) {
        cout << "Constructor\n";
        a = new int;
        *a = x;
    }
    MyObj10(MyObj10 volatile &obj) { // copy constructor
        cout << "Copy Constructor\n";
        a = obj.a;
        // Deep copy
    }

    MyObj10(MyObj10 volatile &&obj) : a{ obj.a } { // copy constructor
        cout << "Move Constructor\n";
        obj.a = nullptr;
    }
};
MyObj10 example10() {
    MyObj10 volatile a = MyObj10(3);;
    return a;
}

/* But here ROV*/
MyObj10 example10_2() {
    MyObj10  a = MyObj10(3);;
    return a;
}

/*  ALL OBJECT IN A PROGRAM HAVE ONE OF THE FOLLOWING STORAGE DURATION
1) AUTAMATIC storage duration:- Thev storage fpr a block variable not declared thread_local, satatic or
extern is allocated at the begning of the enclosing code block and deallocated at the end. The storage 
for a function parameter last until immediately afterits destruction.

2) STATIC storage duration:- The storage for the object is allocated when the program begins and deallocated when
the program end. Only one instance of the object exist. All objects declared at namespace scope (including
global namespace) have this storage duration, plus those declared with static or extern.

3) THREAD storage duration:- The storage for the object is allocated when the thread begins and deallocated when
the thread ends. Each thread has its own instance of the object. On
*/

// g++ -fno-elide-constructors -std=c++20 your_file.cpp


// ANTI FATENS

int main() {

    MyObj2 a = example8();
    return 0;
}