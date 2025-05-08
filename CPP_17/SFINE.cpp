#include <iostream>

struct IRobot{};
struct WalkRobot : IRobot {
    static const char WAKABLE{};
    void Walk() {
        std::cout << "Walking\n";
    }
};

struct FlyRobot : IRobot {
    static const char FLY{};
    void Fly() {
        std::cout << "flying\n";
    }
};

template <class T>
void DoSomething(T& r, int a, decltype(T::WAKABLE)* = nullptr) {
    std::cout << "Walkable Called\n";
    r.Walk();
}


// template <class T>
// void DoSomething(T& r, int a) {
//     std::cout << "Generic Called\n";
//     // r.Fly();
// }

template <class T>
void DoSomething(T& r, int a, decltype(T::FLY)* = nullptr) {
    std::cout << "Flyable Called\n";
    r.Fly();
}


int main() {
    WalkRobot w;
    DoSomething<WalkRobot>(w, 5); // call of overloaded ‘DoSomething<WalkRobot>(WalkRobot&)’ is ambiguous

    FlyRobot f;
    DoSomething(f, 6);
}