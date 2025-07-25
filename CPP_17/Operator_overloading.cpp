#include <iostream>
using namespace std;

// Implicit Conversion Operator: Allows an object to be automatically converted to another type.
struct MyBool {
    bool value;

    // Implicit conversion to bool
    operator bool() const { return value; }
};

void check(MyBool b) {
    if (b) { // Implicitly converts b to bool
        std::cout << "True!\n";
    } else {
        std::cout << "False!\n";
    }
}

void Implicit_conversion() {
    MyBool mb{true};
    check(mb); // Works because of implicit conversion
    /*  Use case: Wrap primitive types in safer types while preserving 
    usability (e.g., std::chrono::duration, std::optional, etc.).*/
}


// Function Call Operator (operator()) Allows an object to be called like a function
struct Multiplier {
    int factor;

    // Function call operator
    int operator()(int x) const {
        return x * factor;
    }
};

void Function_Call() {
    Multiplier times2{2};
    std::cout << times2(10) << "\n";  // Output: 20
    // Use case: Functors, function objects, lambdas, custom predicates, strategy patterns, etc.
}

#include <iostream>

struct Wrapper {
    int value;

    // Implicit conversion to int
    operator int() const { return value; }

    // Function call returns square
    int operator()() const { return value * value; }
};

void Test() {
    Wrapper w{4};

    int x = w + 1;         // Implicit conversion → 4 + 1 = 5
    int y = w();           // Function call → 4 * 4 = 16

    std::cout << "x = " << x << ", y = " << y << "\n"; // Output: x = 5, y = 16
}

// void PrintVector2(const vector2& v);
struct vector2 {
    float x, y;

    vector2(float x, float y) : x(x), y(y) {}

    vector2 Add(const vector2& other) const {
        // return vector2(x + other.x, y + other.y);
        // return *this + other;
        return operator+(other); // This calls vector2::operator+(const vector2&) const
    }

    vector2 operator+(const vector2& other) const {
        // return Add(other);
        return vector2(x + other.x, y + other.y);
    } 

    vector2 Multiply(const vector2& other) const {
        return vector2(x * (other.x), y * (other.y));
    }

    bool operator==(const vector2& other) {
        return this->x ==other.x && y == other.y;
    }

    bool operator!=(const vector2& other) {
        return !(*this == other);
    }

    vector2& operator+=(const vector2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    // void Print() {
    //     PrintVector2(*this);
    // }

    void Print() const;  // Use a forward declaration
};


std::ostream& operator<<(std::ostream& stream, const vector2& other) {
    stream << other.x << ", " << other.y;
    return stream;
}

void PrintVector2(const vector2& v)
{
    std::cout << v << "\n";
}

// 5️⃣ Implement Print() after PrintVector2 is defined
void vector2::Print() const {
    PrintVector2(*this);
}

#define ind 100

void show() {
    #undef ind;
    #define ind 1000
    std::cout << ind << std::endl;  // ind replaced with 100
}


int main() {
    std::cout << ind << std::endl;  // ind replaced with 100
    show();
    
    vector2 position(4.0f, 4.0f);
    vector2 speed(4.0f, 4.0f);
    vector2 powerup(4.0f, 4.0f);

    vector2 res1 = position.Add(speed.Multiply(powerup));
    vector2 res2 = position + speed;
    cout << res2.x << "\n";
    res1.Print();

    Implicit_conversion();

}
