#include <iostream>
#include <utility>

struct S
{
    int x;

    explicit S(int x) :x{x} {
        std::cout << "construct S(" << x << ")\n";
    }

    S(const S &other) : x{other.x} {
        std::cout << "Copy S(" << x << ")\n";
    }
};

S make_value(int x)
{
    return S(x);
}

int main() {
    // S s(21);
    S s = make_value(21);
    std::cout << "Answer: " << s.x << ")\n";
    return 0;
}