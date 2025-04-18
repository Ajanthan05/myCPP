#include <iostream>
#include <vector>
#include <algorithm>

/*
[ captures ] ( parameters ) -> return_type {
    // function body
};


*/

auto add = [](int a, int b) {
    return a + b;
};

/* Capturing this in a class  from c++14*/
class Test {
    int val = 42;
public:
    void show() {
        auto lambda = [this]() {
            std::cout << val << std::endl;
        };
        lambda();
    }
};

int main() {
    std::cout << add(2, 3) << "\n"; // Output: 5

    // If it’s complex or ambiguous, use -> type
    auto divide = [](double a, double b) -> double {
        if (b == 0) return 0;
        return a / b;
    };

    // 1. Capture by Value (=)
    int x = 10;
    auto val_capture = [=]() {
        std::cout << x << std::endl; // captures a *copy* of x
    };

    // 2. Capture by Reference (&)
    int z = 3;
    auto ref_capture = [&]() {
        std::cout << "ref_capture: " << x << std::endl;
        x += 5;
    };
    
    ref_capture();
    std::cout << x; // Output: 15

    // 3. Mixed Capture
    int a = 1, b = 2;
    auto lambda = [a, &b]() {
        // a is captured by value, b by reference
    };

/*
Default Capture Modes
[=] – capture everything by value

[&] – capture everything by reference

[=, &b] – capture everything by value, but b by reference

[&, a] – capture everything by reference, but a by value
*/

    std::vector<int> v = {1, 2, 3, 4, 5};
    std::for_each(v.begin(), v.end(), [](int x) {
        std::cout << x * 2 << " ";
    });

    /* Mutable Lambdas
    If you capture by value but want to modify the captured copy
    
    🔸 Key Points:
x is captured by value, so it's a copy inside the lambda.

mutable allows you to modify that copy.

The original x outside the lambda is unchanged.

✅ When to Use This Pattern
Use this when:

You need to capture a variable's value at the time the lambda is created.

You need to modify the copy, e.g., for temporary calculations or repeated use inside the lambda.

You do not want to modify the original variable outside the lambda.
*/
    int y = 10;
    auto lambda_mutable = [y]() mutable -> double {
        y += 5; // OK because lambda is mutable
        std::cout << y << std::endl; // 15
        return y;
    }; // Output: Inside lambda: 15
    double d = lambda_mutable();
    std::cout << "lambda_mutable: " << y << " return: " << d << std::endl; // still 10




    /* Lambdas as Function Pointers */
    auto add = [](int a, int b) -> int {
        return a + b;
    };
    int (*func_ptr)(int, int) = add;
    std::cout << func_ptr(3, 4); // Output: 7

    /* Generic Lambdas (C++14+) */
    auto generic = [](auto x, auto y) {
        return x + y;
    };
    std::cout << generic(3, 4.5); // Output: 7.5

    
    // ?????????????? Lambdas in template context, constexpr + template parameters, default constructible lambdas
    // for STL algorithms, nested lambdas, or class-based scenarios!
}