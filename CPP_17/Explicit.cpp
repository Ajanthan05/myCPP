#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>

using namespace std;

/*  In C++, the explicit keyword is used to prevent implicit type conversions that might lead to unexpected behavior. It's primarily applied to constructors and, since C++11, to conversion operators as well.*/
class MyClass {
    int b_var;
public:
    MyClass() {}
    // Stop compiler to guessing for us
    explicit MyClass(int var) : b_var(var) {}  
    void Print() {cout << b_var << "\n";}
};
    
void fun(MyClass obj) { 
    obj.Print();
}
    
// explicit with Conversion Operators:- Since C++11, explicit can also be used on conversion operators:
class MyBool {
public:
    explicit operator bool() const { return true; }
};
    
void Conversion_Operators() {
    MyBool b;
    if (b) { }  // ✅ OK, allowed in conditionals
    // bool x = b; // ❌ Error! Must be explicit: bool x = static_cast<bool>(b);
}

// Since C++20: explicit(bool):-   C++20 introduced conditional explicitness:
// This is useful in templates or generic code.
class MyClass2 {
public:
    explicit(true) MyClass2(int x);  // Always explicit
    explicit(false) MyClass2(double x);  // Never explicit (i.e., implicit)
    explicit(sizeof(int) > 2) MyClass2(char x);  // Conditionally explicit
};
  

// explicit: Prevents Misuse

class Connection {
public:
    explicit operator bool() const {
        return isConnected;
    }

    void connect() { isConnected = true; }

private:
    bool isConnected = false;
};

void handleConnection(int status) {
    std::cout << "Handling connection status: " << status << "\n";
}

void PreventsMisuse() {
    Connection conn;

    // handleConnection(conn);  // ❌ Error: can't implicitly convert
    if (conn) {
        std::cout << "Connected!\n";
    } else {
        std::cout << "Not connected.\n";
    }
}
/*✅ Why this is safer:
The explicit keyword stops accidental conversions outside of conditions.
You can still use the object in if (conn) because C++ allows explicit operator bool() in conditionals (thanks to a special rule).*/

// Multiple Arguments, No Defaults 
class Point {
public:
    explicit Point(int x, int y) { /* ... */ }
};

void draw(Point p) { }

void Multiple_Arguments() {
    // draw(10);       // ❌ Error: No constructor matches
    /*{10, 20} → means: "Hey, compiler, implicitly create a Point from these two numbers."
    If the constructor is explicit, the compiler refuses to do it implicitly.
    draw({10, 20}); // Not OK   */

    draw(Point{10, 20});  // ✅ OK, this is explicit
    Point p(5,6);
    draw(p);
}

// Multiple Arguments With Defaults → explicit Matters
class Point {
public:
    explicit Point(int x, int y = 0) { /* ... */ }
};

void draw2(Point p) { }

void With_Defaults() {
    // Without explicit, You May Get Unwanted Conversions
    // draw2(5);        // ❌ Error: Needs explicit constructor call
    draw2(Point(5)); // ✅ OK
    /*  Here, Point(int x, int y = 0) can be called with one argument, 
    so implicit conversion is possible. That’s where explicit blocks it.*/
}



int main() {
    MyClass obj1(10);   // Noramal call to the constructor
    // MyClass obj2 = 20;  // Implicit call to constructor // here the type is same

    fun(obj1);          // Noramal call to the constructor
    // fun(42);            // Implicit call to the constructor  // Works because MyClass(int) is not explicit
    fun(MyClass(5));

    // ListInatiolization :-  With curlybrase we are not alowed for narowing
    // MyClass obj_float{10.2345325f}; //error: narrowing conversion of ‘1.02345324e+1f’ from ‘float’ to ‘int’

    Multiple_Arguments();

    return 0;
}