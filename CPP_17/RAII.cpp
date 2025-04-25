#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>

using namespace std;

// Resource Acquisition is Initialization
class Collection {
public:
    Collection() {
        data = new int[10];
    }
    ~Collection() {
        delete[] data;
    }

    int& operator[](std::size_t idx) {
        return data[idx];
    }

    const int& operator[](std::size_t idx) const {
        return data[idx];
    }

private:
    int* data;
};

int main() {
    Collection my;
    my[0] = 1;
    my[3] = 3;
    my[4] = 5;
    cout << my[4] << "\n";

    return 0;
}