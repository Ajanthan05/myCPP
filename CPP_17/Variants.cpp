#include <iostream>
#include <variant>

using VariantType = std::variant<int, float, std::string>;

void print_variant(const VariantType& v) {
    std::visit([](auto&& arg) { std::cout << arg << std::endl; }, v);
}

int main() {
    VariantType v = 42;
    print_variant(v); // prints 42

    v = 3.14f;
    print_variant(v); // prints 3.14

    v = "Hello, Variant!";
    print_variant(v); // prints Hello, Variant!
}
