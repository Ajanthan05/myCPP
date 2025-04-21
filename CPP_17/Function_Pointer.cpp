#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>

using namespace std;

/*  A function pointer is a pointer that points to a function instead of 
data. You can use it to call a function dynamically, pass functions as 
arguments, or store functions in arrays.

return_type (*pointer_name)(parameter_types);

*/
int add(int a, int b) {
    return a + b;
}

// Declare the function pointer globally
int (*func_ptr)(int, int);

// Assign the function to the pointer
// Not Allowed: Assignment at Global Scope Without const
// func_ptr = &add;  // or just add
// This will cause a compiler error unless func_ptr is const and add is declared before this line
// int (*func_ptr)(int, int) = &add;  // ❌ May error if 'add' not yet visible

void functoin_ptr() {
    func_ptr = &add;  // or just add

    // Call the function through the pointer
    int result = func_ptr(2, 3);  // result = 5
}

/*
🔸 Why use Function Pointers?
Call functions dynamically

Pass a function to another function (callback style)

Array of function pointers for menus or operations

Implementing polymorphism in C
*/

// 🔹 Example: Passing Function as Argument
void process(int x, int (*op)(int)) {
    cout << "Result: " << op(x) << endl;
}

/*  NOTE
the & is actually optional when referring to a function. Both of these are valid and equivalent:

c
Copy
Edit
int (*function_ptr)(int, int) = add;     // ✅ No & needed
int (*function_ptr)(int, int) = &add;    // ✅ With &
This is because the function name add decays to a pointer to the function automatically when used in this context 
(except when you're calling it or using sizeof or &add explicitly).
*/

int square(int x) { return x * x; }
int cube(int x)   { return x * x * x; }

void Function_as_Argument() {
    process(3, square); // Result: 9
    process(3, cube);   // Result: 27
}


// 🔹 Array of Function Pointers
int subtract(int a, int b) {return a-b;}

int (*options[2])(int, int) = {add, subtract};

void array_fun_ptr() {
    int x=5;
    int y=6;
    cout << options[1](y, x);
}

// 🔹 Typedef for Clean Syntax  Using typedef or using (C++11) can make your life easier:



int main() {
    Function_as_Argument();
    array_fun_ptr();
    cout << "\n";
}