#include <iostream>
#include <iomanip>

/*  conversion operators and conversion constructors are mechanisms that allow objects of a class to be 
converted to or constructed from objects of other types. These conversions can be implicit or explicit, 
and they play a key role in making user-defined types behave more like built-in typ

A conversion constructor is a constructor that can be called with a single argument (or has default values 
for all arguments except the first). It is used to convert a value of a different type to an object of the class.

A conversion constructor is a constructor that can be invoked with a single argument (or with default values for other arguments). 
It enables implicit or explicit conversion from another type to the class type.*/
class MyClass {
    int value;
public:
    MyClass(int x) : value(x) {}
    void display() { std::cout << value << std::endl; }
};

int T() {
    MyClass obj = 10; // Implicit call to conversion constructor
    obj.display();    // Output: 10
}

class MyClass2 {
    int value;
public:
    explicit MyClass2(int x) : value(x) {}
};
int T1() {
    // MyClass2 obj1 = 10;  // ❌ Error if constructor is explicit

    // To prevent implicit conversions, use the explicit keyword.

    MyClass2 obj2(10);   // ✅ OK
}


/*  ✅ 2. Conversion Operator (Type-Cast Operator)
A conversion operator is a special member function that allows an object to be converted to another type.
operator <type>() const;

A conversion operator is a special member function that allows an object of a class to be implicitly or explicitly converted to another type. It has the form operator Type().*/
class MyClass3 {
    int value;
public:
    MyClass3(int x) : value(x) {}
    explicit operator int() const { return value; }
};

int main() {
    MyClass3 obj(42);
    // without explicitint x = obj;  // Implicit conversion using operator int()
    // std::cout << x << std::endl;  // Output: 42

    int x = static_cast<int>(obj);  // ✅ OK
    // int y = obj;                 // ❌ Error if operator is explicit
}


/*  ✅ Best Practices
Use explicit to avoid unintended implicit conversions.

Prefer explicit for constructors or operators unless you intend implicit conversions.

Avoid defining both conversion constructor and conversion operator that interact with each other in a confusing way, to reduce ambiguity.*/













class Money {
    long cents;  // store internally as integer cents for precision
public:
    // Conversion constructor
    explicit Money(double amount) {
        cents = static_cast<long>(amount * 100); // 2 decimal precision
    }

    // Conversion operator
    explicit operator double() const {
        return static_cast<double>(cents) / 100.0;
    }

    void display() const {
        std::cout << "$" << std::fixed << std::setprecision(2)
                  << static_cast<double>(*this) << "\n";
    }
};
