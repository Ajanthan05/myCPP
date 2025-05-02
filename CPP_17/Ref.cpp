#include <functional>
#include <iostream>

/*  When you call std::ref(obj), it creates a std::reference_wrapper<T> that behaves like a reference.
std::thread t(my_function, std::ref(a)); // passes 'a' by reference

    ✅ When you want references inside places that expect copies, like:
std::thread
std::async
std::bind
std::for_each
passing to lambdas with value captures*/

void print_sum(int a, int b) {
    std::cout << a + b << "\n";
}

// c. Bind function
void Bind() {
    int x = 10, y = 20;
    auto f = std::bind(print_sum, std::ref(x), std::ref(y));
    x = 30;
    y = 40;
    f(); // prints 70
}
// Here, std::ref ensures the latest value of x and y is used.

int main() {
    int x = 42;
    auto ref_x = std::ref(x);

    // Access value
    std::cout << ref_x.get() << std::endl; // prints 42

    ref_x.get() = 100;  // modifies x directly
    std::cout << x << std::endl; // prints 100

////////////////////////////////////////////////
    int x2 = 10, y2 = 20;
    auto f = std::bind(print_sum, std::ref(x2), std::ref(y2));
    x2 = 30;
    y2 = 40;
    f(); // prints 70


//////////////  for_each ///////////////////
    std::vector<int> v = {1, 2, 3};
    int sum = 0;

    std::for_each(v.begin(), v.end(), [&](int n) { sum += n; });
    std::cout << "Sum: " << sum << "\n"; // 6

/*      [&] — capture everything by reference (including sum).
int n — n is passed by value (each element from v is copied into n).

So:
sum is captured by reference (due to [&]), no need for std::ref.
n is copied from the vector element into the lambda argument.

Why no std::ref needed in your example?
Because you are just: iterating over std::vector<int>.
using a reference capture ([&]) for sum.
copying each int into n, and that's fine.
No special reference handling is needed here.*/

// So When it needed
    int a = 1, b = 2, c = 3;
    std::vector<std::reference_wrapper<int>> v1 = { std::ref(a), std::ref(b), std::ref(c) };

    std::for_each(v1.begin(), v1.end(), [](std::reference_wrapper<int> n) {
        n.get() += 10;
    });

    std::cout << a << " " << b << " " << c << std::endl; // prints 11 12 13

    
// std::cref(obj) is like std::ref but makes a const reference instead.
    auto obj_con = std::cref(a);

    /*   Limitations
std::ref cannot wrap a temporary. Example:
std::ref(5); // Error: can't bind non-const reference to rvalue
You must wrap lvalues (named variables).*/



}
