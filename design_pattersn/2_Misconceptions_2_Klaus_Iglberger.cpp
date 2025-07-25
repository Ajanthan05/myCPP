#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>

using namespace std;

/*  🧩 Why Use a Friend Function?
Use a friend function when:

You need external access to a class’s internals, but you don’t want it to be a member.

You want to support operator overloading where the left-hand operand is not the class itself.

You need tight coupling between two or more classes (like helper functions).

📦 Key Properties
Declared inside the class with friend, but defined outside.
Not a member function, so it doesn’t use this.
Can access private/protected members.
Friendship is not mutual or inherited.
*/

class MyClass {
private:
    int secret;

public:
    MyClass(int s) : secret(s) {}

    friend void revealSecret(const MyClass& obj);  // friend declaration
};
void revealSecret(const MyClass& obj) {
    std::cout << "Secret is: " << obj.secret << std::endl;  // OK: has access
}

/*  Common Use Case: Operator Overloading
When you want to overload +, ==, etc., and the left-hand operand is not your class type*/
class Point {
private:
    int x, y;

public:
    Point(int x, int y) : x(x), y(y) {}

    friend Point operator+(const Point& a, const Point& b);
};

Point operator+(const Point& a, const Point& b) {
    return Point(a.x + b.x, a.y + b.y);
}

class Engine;
class Car {
private:
    int speed = 200;

    friend class Engine;
};

/*   Best Practices
Use friendship sparingly — it breaks encapsulation.   ???
Favor member functions or public interfaces when possible.

Ideal for:
    Operator overloads
    Utility functions that need internal access
    Tight collaboration between helper classes

*/

/*  Friend with Operator Overloading
Overload << to print a class*/

class Box {
    int length;

public:
    Box(int l) : length(l) {}

    friend std::ostream& operator<<(std::ostream& os, const Box& b);
};

std::ostream& operator<<(std::ostream& os, const Box& b) {
    os << "Box length: " << b.length;
    return os;
}
void T_() {
    Box b(42);
    std::cout << b;  // Prints: Box length: 42
}


/*  3. Friend Class Example  */
class Database;

class Logger {
public:
    void logSecret(const Database& db);
};

class Database {
private:
    std::string password = "top_secret";

    friend class Logger;
};

void Logger::logSecret(const Database& db) {
    std::cout << "Logging password: " << db.password << std::endl;
}


/*  4. Friend Function in Template Class*/
template<typename T>
class Wrapper {
    T value;

public:
    Wrapper(T val) : value(val) {}

    friend void printValue<>(const Wrapper<T>& obj);  // declare template friend
};

template<typename T>
void printValue(const Wrapper<T>& obj) {
    std::cout << "Value: " << obj.value << std::endl;
}


/*   5. Mutual Friendship (Not Automatic)  
Friendship is not symmetric — both sides must declare friend if mutual access is needed. */
class A;

class B {
    friend class A;  // A can access B
private:
    int valB = 20;
};

class A {
    friend class B;  // B can access A
private:
    int valA = 10;
};






class Engine {
public:
    void showSpeed(const Car& c) {
        std::cout << c.speed << std::endl; // Accesses private member
    }
};


template< typename Derived >
class Animal {
private:
/*  This:
Prevents anyone from creating or destroying an Animal<Derived> directly.
Enforces that only Derived (like Sheep) can create an instance of itself.
🔐 It’s a way to enforce correct inheritance and usage.*/

    Animal() = default;     // Protects against wrong Derived class
    ~Animal() = default;
    friend Derived;

public:
    void make_sound() const {
        static_cast<Derived const&>(*this).male_sound();
    }
};
class Sheep : public Animal<Sheep>
{
public:
    // ~Sheep(); Remember the Rule of 5

    void make_sound() const { cout << "Baaaa\n"; }
};


/*  LIMITATIONS: There is no common base class
Every drive class has different base class*/

/*  Adding Functionality with C++23 
No template parameter anymore*/
struct NumericalFunctions {
    // Explicit object parameter (aka "Deducing This")
    void scale( this auto&& self, double multiplicator ) {
        self.setValue(self.getValue() * multiplicator);
    }
};

struct Sensitivity : public NumericalFunctions {
    double getValue() const {return value; }
    void setValue( double v ) { value = v; }
    double value;
};
void T_23() {
    Sensitivity s{1.2};
    s.scale( 2.0 );

    std::println( std::cout, "s.getValue() = {}", s.getValue() );
}


int main() {


    return 0;
}