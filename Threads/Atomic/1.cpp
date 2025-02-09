#include <iostream>
#include <thread>
#include <atomic>

/*
atomic<*>
Nither copy constructible nor copy assignable
Can assign non atomic Booleans and can be construct using non atomic Booleans

But can assign non atomic Booleans and can be construct using
non atomic Booleans.

All attomic other than attomic flags
is_lock_free
store
load
exchange :- Replace the stored value with new one automatically retrieve the original one
compare_exchange_weak
compare_exchange_strong
*/

void atomicFlag() {
    // std::atomic_flag flag2 = true; An "ATC check-in" for a flight refers to the process where a pilot communicates with Air Traffic Control (ATC) to inform them of their aircraft's presence, position, and intentions, typically when entering a new airspace or reaching a specific point in their flight plan, essentially "checking in" with ATC to receive instructions and clearances for safe navigation. 
    std::atomic_flag flag2 = ATOMIC_FLAG_INIT;

    // Set the flag value to true and return previous value
    std::cout << "1st_previous flag value: " << flag2.test_and_set() << "\n";
    std::cout << "2nd_previous flag value: " << flag2.test_and_set() << "\n";
    std::cout << "3rd_previous flag value: " << flag2.test_and_set() << "\n";

    // clear the flag value
    flag2.clear();
    std::cout << "After clear it return previous flag value: " << flag2.test_and_set() << "\n";
    std::cout << "After clear an  test_and_set() it return previous flag value: " << flag2.test_and_set() << "\n";
}

void run_code() {
    std::cout << "Start" << std::endl;
    std::atomic<bool> flag_1;   // flag_1 assign true bt default
    std::cout << "flag 1 = " << flag_1 << std::endl;

    // Cannot copy construct
    // std::atomic<bool> flag_2(flag_1);

    // cannot copy assignable
    // std::atomic<bool> flag_3 = flag_1;

    // construct using non atomic boolean value
    bool non_atomic_bool = true;
    std::atomic<bool> flag_4(non_atomic_bool);
    std::cout << "flag 4 = " << flag_4 << std::endl;

    // assine  non atomic boolean value
    std::atomic<bool> flag_5 = non_atomic_bool;
    std::cout << "flag 5 = " << flag_5 << std::endl;
}


void run_code2() {
    std::atomic<bool> x(false);
    std::cout << "Atomic boolean is implemented lock free - " 
        << (x.is_lock_free() ? "Yes": "No") << "\n";

    // It lock free. But if we use user define type of atomic template 
    // Most definitely compiler will use internal lock to provide the atomicity of operations


    std::atomic<bool> y(true);

    std::cout << "Atomic boolean is implemented lock free - " 
        << (y.is_lock_free() ? "Yes": "No") << "\n";

    // store operations
    x.store(false);  // Atomic type
    x.store(y);      // non atomic type an be used in store operation

    // Load operations
    std::cout << "value of atomic bool y - " << y.load() << "\n";

    // Exchange operation
    bool z = x.exchange(false);
    std::cout << "Current value of atomic bool x - " << x.load() << "\n";
    std::cout << "Previous value of atomic bool x - " << z << "\n";
}

/*
1. compare_exchange_weak()
2. compare_exchange_strong()

bool r = x.compare_exchange_weak(T& expected, T desired)

Compares the values of the atomic variable(x) with supllied
expected(T& expected) value and stores the supplied desired
value (T desired) if they are equal. If they are not equal the
expected value is updated with the actual value of the atomic
variable. This will return true if store is performed false otherwise.
std::cout <<  << "\n";
*/
void run_compare_exchange() {
    std::atomic<int> x(20);

    int expected_value = 10;
    std::cout << "expected_value: " << expected_value << "\n";

    bool return_val = x.compare_exchange_weak(expected_value, 20);
    std::cout << "Operation sucessfull :" << (return_val ? "Yes" : "No") << "\n";
    std::cout << "Current expected val :" << expected_value << "\n";
    std::cout << "Current x            :" << x.load() << "\n";

    /*
    expected_value: 10
    Operation sucessfull :No
    Current expected val :20
    Current x            :20*/

    /*
    compare_exchange_week doesnot guatantee the successdulness of the operation.expected_value,
    even if the expectde value is equal to the atomic variable

    It the machine does not have a single compar exchange instruction That processor will not 
    be able to guarantee the atomic of the operation

    There is no atomic multiplay cpp wont allow it
   Not atomic 
    x *= 2; // No atomic multiplication!
    x = x+1;
    x = x*2;
    */

}

int main() {
    // atomicFlag();
    // run_code2();

    run_compare_exchange();

    return 0;
}