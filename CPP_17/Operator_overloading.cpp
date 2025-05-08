#include <iostream>

// Implicit Conversion Operator: Allows an object to be automatically converted to another type.
struct MyBool {
    bool value;

    // Implicit conversion to bool
    operator bool() const { return value; }
};

void check(MyBool b) {
    if (b) { // Implicitly converts b to bool
        std::cout << "True!\n";
    } else {
        std::cout << "False!\n";
    }
}

void Implicit_conversion() {
    MyBool mb{true};
    check(mb); // Works because of implicit conversion
    /*  Use case: Wrap primitive types in safer types while preserving 
    usability (e.g., std::chrono::duration, std::optional, etc.).*/
}


// Function Call Operator (operator()) Allows an object to be called like a function
struct Multiplier {
    int factor;

    // Function call operator
    int operator()(int x) const {
        return x * factor;
    }
};

void Function_Call() {
    Multiplier times2{2};
    std::cout << times2(10) << "\n";  // Output: 20
    // Use case: Functors, function objects, lambdas, custom predicates, strategy patterns, etc.
}

#include <iostream>

struct Wrapper {
    int value;

    // Implicit conversion to int
    operator int() const { return value; }

    // Function call returns square
    int operator()() const { return value * value; }
};

void Test() {
    Wrapper w{4};

    int x = w + 1;         // Implicit conversion → 4 + 1 = 5
    int y = w();           // Function call → 4 * 4 = 16

    std::cout << "x = " << x << ", y = " << y << "\n"; // Output: x = 5, y = 16
}


int main() {
    Implicit_conversion();

}
