#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>

using namespace std;

/* You return an object of a class in C++ when
1. You want to produce a result from a function that the caller will use.
*/

// Returns an instance of a class (like a smart constructor).
class Point {
public:
    int x, y;
    Point(int x, int y) : x(x), y(y) {}
};

Point make_point() {
    return Point(3, 4);  // RVO will kick in
}

// 2. Operator Overloading      Returning a result of a computation.
class Vector {
public:
    int x, y;
    Vector(int x, int y) : x(x), y(y) {}
    
    Vector operator+(const Vector& other) const {
        return Vector(x + other.x, y + other.y);
    }
};

// 3. Builder Pattern / Fluent API  Returning *this or a new object to allow chaining.
class QueryBuilder {
public:
    std::string query;

    QueryBuilder& select(std::string col) {
        query += "SELECT " + col + " ";
        return *this;
    }

    QueryBuilder& from(std::string table) {
        query += "FROM " + table + " ";
        return *this;
    }

    std::string build() {
        return query;
    }
};
    
// 4. Returning by Value for ImmutabilityInstead of modifying input objects, return new ones.
std::string makeGreeting(std::string name) {
    return "Hello, " + name;
}

// 5. Custom Wrapper or Result Object   Encapsulate multiple return values or status info.
struct Result {
    bool success;
    std::string message;
};

Result processData() {
    return {true, "Processed successfully"};
}

/*
Purpose	Example
To create a new object	return ClassName(args);
To perform computation	return obj + other;
To enable method chaining	return *this;
To encapsulate results	return {success, "msg"};
*/


int main() {


    return 0;
}