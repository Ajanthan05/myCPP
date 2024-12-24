#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

class Widget {
private:
    int i{0};
    std::string s{};
    unique_ptr<int> pi{};

public:
    // ...

    // Move sonstructor
    Widget(Widget&& w) = default;

    // Move assignment operator
    Widget& operator=(Widget&& w) = default;

};

int main() {


    return 0;
}