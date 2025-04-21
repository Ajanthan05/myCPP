#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Wrapper to track string copy/move
struct TrackedString {
    string value;

    TrackedString(const string& s) : value(s) {
        cout << "[copy ctor] from std::string: " << value << endl;
    }

    TrackedString(string&& s) noexcept : value(move(s)) {
        cout << "[move ctor] from std::string&&: " << value << endl;
    }

    TrackedString(const char* s) : value(s) {
        cout << "[ctor] from const char*: " << value << endl;
    }

    TrackedString(const TrackedString& other) : value(other.value) {
        cout << "[copy ctor] from TrackedString: " << value << endl;
    }

    TrackedString(TrackedString&& other) noexcept : value(move(other.value)) {
        cout << "[move ctor] from TrackedString: " << value << endl;
    }

    TrackedString& operator=(const TrackedString&) = delete;
    TrackedString& operator=(TrackedString&&) = delete;
};

struct foo {
    template<typename STR>
    foo(int i, double d, char c, STR&& s)
        : i(i), d(d), c(c), s(forward<STR>(s)) {
        cout << "Constructed foo: " << this->s.value << "\n";
    }

    int i;
    double d;
    char c;
    TrackedString s;
};

class bar_O {
    vector<foo> foos;
public:
    template<typename... T>
    void add(T&&... t) {
        foos.emplace_back(forward<T>(t)...);
    }
};

int main() {
    bar_O b;

    const char* cstr = "const char* string";
    b.add(1, 1.1, 'A', cstr); // const char*

    b.add(2, 2.2, 'B', "string literal"); // string literal

    std::string str = "std::string lvalue";
    b.add(3, 3.3, 'C', str); // lvalue

    b.add(4, 4.4, 'D', std::string("std::string rvalue")); // rvalue

    return 0;
}
