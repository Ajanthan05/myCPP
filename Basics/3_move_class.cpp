#include <string>
#include <utility> // for std::move and std::exchange

class Widget {
private:
    int i{0};
    std::string s{};
    int* pi{nullptr};

public:
    // Default constructor
    Widget() = default;

    // Parameterized constructor
    Widget(int i_val, const std::string& s_val, int* pi_val)
        : i(i_val), s(s_val), pi(pi_val) {}

    // Copy constructor
    Widget(const Widget& w)
        : i(w.i), s(w.s), pi(w.pi ? new int(*w.pi) : nullptr) {}

    // Copy assignment operator
    Widget& operator=(const Widget& w) {
        if (this != &w) {
            i = w.i;
            s = w.s;
            delete pi;
            pi = w.pi ? new int(*w.pi) : nullptr;
        }
        return *this;
    }

    // Move constructor
    Widget(Widget&& w) noexcept
        : i(std::move(w.i))
        , s(std::move(w.s))
        , pi(std::exchange(w.pi, nullptr)) {}

    // Move assignment operator
    Widget& operator=(Widget&& w) noexcept {
        if (this != &w) {
            i = std::move(w.i);
            s = std::move(w.s);
            delete pi;
            pi = std::exchange(w.pi, nullptr);
        }
        return *this;
    }

    // Destructor
    ~Widget() {
        delete pi;
    }
};


int main() {

    std::string s("Long string that needs to be copied");
    std::vector<Widget> v{};

    contexpr size_t N(10000);

    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
    start = std::chrono::high_resolution_clock::now();
    

    return 0;
}