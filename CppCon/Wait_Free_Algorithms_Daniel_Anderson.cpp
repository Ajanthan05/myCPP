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

auto current_val = counter.load();
compare_exchange(expectec&, desired) {
    if (current_val == expected) {
        current_val = desired;
        return true;
    }    
    else {
        expected = current_val;
        return false;
    }
}
*/

struct Counter {
public:
// If the counter is greater than zero, add one and return true
// Otherwise do nothing and retun false
    bool increment_if_not_zero() {
        auto current = counter.load();
        while (current>0 && !counter.compare_exchange_weak(current, current + 1)) {}
        return current > 0;
    }

// Decrement the counter. If the counter now equals zero-> return true; 
//                                                    else return false;
//  Precondition: The counter is not zero
    bool decrement() {
        return counter.fetch_sub(1) == 1;  // First fetch
    }

    uint64_t read() { return counter.load(); }

private:
    std::atomic<uint64_t> counter{1};
};


/* Steal some high-order bits of the countet to use as flags
    An operation wants to annonce that the counter has been set to zero
by setting the top flag
(TO any reading thread, top flag = 1 impies that the counter is zero)
    The second flag will be used for helping.
*/

struct Counter {
public:
    static constexpr uint64_t is_zero = 1ull << 63;
    static constexpr uint64_t helped  = 1ull << 62;
    bool increment_if_not_zero() {
        return (counter.fetch_add(1) & is_zero) == 0;
    }

    bool decrement() {
        if (counter.fetch_sub(1) == 1) { // Careful: there could be multiple threads all asleep after executiong this step
            uint64_t e = 0;
            // return counter.compare_exchange_strong(e, is_zero);
            // Solution: One and the only one decrement must take cradit for zeriong the counter
            if (counter.compare_exchange_strong(e, is_zero)) return true;
            else if ((e & helped) && (counter.exchange(is_zero) & helped)) return true;
        }
        return false;
    }

    uint64_t read() { 
        auto val = counter.load();
/*  return counter.compare_exchange_strong(e, is_zero);
There must be a thread about to do this
Let's help them*/
        if (val == 0 && counter.compare_exchange_strong(val, is_zero | helped)) return 0;
/* Problem is, If a read healps to set the is_zero flag, none of the 
decrements return true */
        return (val & is_zero) ? 0 : val;
    }

protected:
    std::atomic<uint64_t> counter{1};
};
/*
Refered as sticky counter (it gets struck at zero)
Required by std::week_ptr<T>::lock
Useful in atomic memory management/ concurant data structor

But didn't we increment the counter?

Yes — physically.

This is the subtle part.

You might expect:

"If it returns false, it must not modify the counter."

That is not what this implementation guarantees internally.

It changes:

is_zero

into:

is_zero + 1

But the zero flag is still present:

1000....0001
^
Z = 1

Therefore read() still returns:

0

because:

(val & is_zero) != 0

Hence:

internal state:
    is_zero + 1

logical state:
    0


*/


#include <atomic>
#include <cstdint>
#include <cassert>

class StickyCounter {
public:
    static constexpr uint64_t is_zero  = 1ull << 63;
    static constexpr uint64_t is_help  = 1ull << 62;
    
    // Construct with initial count (must be >=1).
    explicit StickyCounter(uint64_t init = 1) 
        : counter((init & ~(is_zero|is_help)) | (init==0 ? is_zero|is_help : 0)) 
    {
        assert((init & (is_zero|is_help)) == 0);
    }

    // Returns true if increment succeeded (count was >0).
    bool increment_if_not_zero() noexcept {
        uint64_t old = counter.fetch_add(1, std::memory_order_relaxed);
        return (old & is_zero) == 0;
    }

    // Returns true iff this call made the count reach zero.
    bool decrement() noexcept {
        // Decrement and check if we think it hit zero.
        if (counter.fetch_sub(1, std::memory_order_relaxed) == 1) {
            uint64_t exp = 0;
            // Try to set the zero flag.
            if (counter.compare_exchange_strong(exp, is_zero, 
                    std::memory_order_relaxed, std::memory_order_relaxed)) {
                return true;  // We won the race: count is now stuck at zero.
            }
            // CAS failed: check if a helper flag is set.
            if ((exp & is_help) && 
                (counter.exchange(is_zero, std::memory_order_relaxed) & is_help)) 
            {
                // We saw the help bit: take credit for zero.
                return true;
            }
        }
        return false;
    }

    // Returns the current count (0 if logically zero).
    uint64_t read() noexcept {
        uint64_t val = counter.load(std::memory_order_relaxed);
        if (val == 0) {
            // Help set the zero flag if still zero.
            uint64_t expect = 0;
            if (counter.compare_exchange_strong(expect, is_zero | is_help,
                    std::memory_order_relaxed, std::memory_order_relaxed)) {
                return 0;
            }
            val = expect;
        }
        return (val & is_zero) ? 0 : (val & ~(is_zero|is_help));
    }

private:
    std::atomic<uint64_t> counter;
};

int main() {


    return 0;
}