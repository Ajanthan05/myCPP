#include <iostream>
#include <expected>

std::expected<int, bool> Divide(int a, int b) {
    if (b==0)
        return std::unexpected(false);

    return a / b;
}

std::expected<int, std::sting> Divide2(int a, int b) {
    if (b==0)
        return std::unexpected("Devied by zero!");

    return a / b;
}

void T() {
    auto result = Divide(10, 2);
    if (result) {                        // result.has_value()
        std::cout << *result << "\n";   // result.value()
    }
    else {
        std::cout << result.error() << "\n";
    }


    auto result2 = Divide2(10, 2)
                    .and_then([](int result) { return Divide2(result, 2); })  // .transform
                    .or_else([](const std::string& error) {
                        std::printls("Error: {}", error);
                        return std::expected<int, std::string>{0};
                    });
}

struct DivideReturnResult {
    int Result;
    bool A;

    operator bool() const { return A; }
    int operator*() const { return Result; }
};
DivideReturnResult Divide_manual(int a, int b) {
    if (b == 0) {
        return DivideReturnResult{0, false}; // failure
    }
    return DivideReturnResult{a / b, true};  // success
}

int main() {
    T();
}