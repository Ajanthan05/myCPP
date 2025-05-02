#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>

#include <functional>
using namespace std;

/*  Function Pointer in C++
return_type (*FuncPtr) (parameter type, ....);  
*/


/*  Functions:- store and invoke callable entities like functions, lambda expressions, bind expressions, or function objects (functors).
std::function<ReturnType(Args...)> func;
✅ Key Features
Type-erased wrapper: Can store any callable type with a matching signature.

Copyable and Assignable: Unlike raw function pointers or lambdas with captures.

Polymorphic Behavior: Behaves like virtual functions without inheritance.

Nullable: Can be empty, similar to a null pointer. You can check with if (func).

*/
int add(int a, int b) {return a+b;}

// 3. Wrap a functor (object with operator())
struct Multiply {
    int operator()(int a, int b) const { return a * b; }

    int mult(int x, int y) { return x*y; }
};
void e3() {
    std::function<int(int, int)> f = Multiply();
    std::cout << f(4, 5);
}

void e4() {
    Multiply obj;
    // std::function<int(Multiply&, int, int)> f = &Multiply::mult; this work or
    std::function<int(Multiply*, int, int)> f = &Multiply::mult;
    cout << f(&obj, 5, 6) << "\n";
}

void examples() {
    // 1. Wrap a free function
    std::function<int(int, int)> f = add;
    cout << f(2,3) << "\n"; 

    // 2. Wrap a lambda
    auto lambda = [](int a, int b) { return a * b; };
    std::function<int(int, int)> f2 = lambda;
    std::cout << f2(3, 4);


}


//////////////////  BIND    /////////////////////
/*  std::bind:- binding arguments to a function, function pointer, lambda, or member function.
It allows:
Partial function application (pre-filling some arguments)
Reordering of arguments
Adapting member functions or functors to standard function signatures
std::bind(callable, arg1, arg2, ..., argN)
callable: any callable entity (function pointer, lambda, functor, member function).
argX: can be a value, a std::placeholders::_1, _2, etc.*/


int main() {

    e4();
    return 0;
}