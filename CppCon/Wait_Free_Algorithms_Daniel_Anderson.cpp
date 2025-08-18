#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>

#include <atomic>

using namespace std;

/*
bool compare_exchange_week(expected&, desired) {
    if (current_value == expected) {
        current_value = desired; return true;
    }
    else {
        expected = current_value; return false;
    }
}
*/

struct Counter {
public:
    bool increment_if_not_zero() {
        auto current = counter.load();
        while (current>0 && !counter.compare_exchange_weak(current, current + 1)) {}
        return current > 0;
    }

    bool decrement() {
        return counter.fetch_sub(1) == 1;  // First fetch
    }

    uint64_t read() { return counter.load(); }

private:
    std::atomic<uint64_t> counter{1};
};

struct Counter {
public:
    static constexpr uint64_t is_zero = 1ull << 63;

    bool increment_if_not_zero() {
        return (current.fetch_add(1) & is_zero) == 0;
    }

    bool decrement() {
        if (counter.fetch_sub(1) == 1) {
            uint64_t e = 0;
            return counter.compare_exchange_strong(e, is_zero);
        }
        return false;
    }

    uint64_t read() { 
        auto val = counter.load();
        return (val & is_zero) ? 0 : val;
    }

private:
    std::atomic<uint64_t> counter{1};
};

int main() {


    return 0;
}