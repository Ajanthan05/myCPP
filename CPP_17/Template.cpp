#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>

using namespace std;

/* variadic template:- allows a function or class to accept any number and any types of arguments.
Args... is a template parameter pack. (List of types)

*/


template <typename... Args>
void func(Args... args) {
    // Args... becomes <int, double, const char*, std::string>.
    // args... becomes the actual values passed.

}

void Parameter_pack() {
    func(1, 3.14, "hello", std::string("world"));
}

// In std::forward<Args>(args)...
template <typename... Args>
void call_func(Args&&... args) {
    func(std::forward<Args>(args)...);
    // Args&&... is a forwarding reference pack (Perfact for universal references)
    // std::forward<Args>(args)... expands into
    // std::forward<Arg1>(arg1), std::forward<Arg2>(arg2), ...

}

// Logging Utility (Variadic Print) A logging function that prints any number of arguments.
void log() {
    std::cout << std::endl;
}

template <typename T, typename... Args>
void log(T&& first, Args&&... rest) {
    std::cout << std::forward<T>(first) << " ";
    log(std::forward<Args>(rest)...);
}

int main() {
    // Flexible: Supports any number and type of arguments.
    log("Error:", 404, "Not Found", 3.14);

    return 0;
}