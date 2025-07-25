#include <coroutine>
#include <iostream>
using namespace std;

struct ReturnObject {
    struct promise_type {
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; } // Changed to suspend_always

        ReturnObject get_return_object() {
            return { std::coroutine_handle<promise_type>::from_promise(*this) };
        }
        void unhandled_exception() {}

        double get_value() const noexcept { return value; }
        void set_value(double val) noexcept { value = val; }

    private:
        double value{3.14};
    };

    std::coroutine_handle<promise_type> handle;
    ReturnObject(std::coroutine_handle<promise_type> h) : handle{h} {}

    operator std::coroutine_handle<promise_type>() const { return handle; }
    ~ReturnObject() {
        if (handle) handle.destroy(); // Destroy handle in destructor
    }
};

ReturnObject DataAccessCoro() {
    cout << "1. hello from coroutine\n";
    co_await std::suspend_always{};

    cout << "2. hello from coroutine\n";
    co_await std::suspend_always{};

    cout << "3. hello from coroutine\n";
    co_await std::suspend_always{};
}

double caller() {
    ReturnObject coro = DataAccessCoro();
    std::coroutine_handle<ReturnObject::promise_type> handle = coro;

    // Get the promise value before resuming
    double result = handle.promise().get_value();

    // Resume the coroutine until it completes
    while (!handle.done()) {
        handle.resume();
    }

    // No explicit destroy here; handled by ReturnObject destructor
    return result;
}

void T() {
    double val = caller();
    cout << "Value from Coroutine: " << val << "\n";
}


int i = 0;
struct ReturnObjectReturn {
    struct promise_type {

        void return_void() {
            cout << i++ << ". promise_type::return_void\n";
        }

        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; } // Changed to suspend_always

        ReturnObjectReturn get_return_object() {
            return { std::coroutine_handle<promise_type>::from_promise(*this) };
        }
        void unhandled_exception() {}
    };

    std::coroutine_handle<promise_type> handle;
    ReturnObjectReturn(std::coroutine_handle<promise_type> h) : handle{h} {}

    operator std::coroutine_handle<promise_type>() const { return handle; }
    ~ReturnObjectReturn() {
        if (handle) handle.destroy(); // Destroy handle in destructor
    }
};
ReturnObjectReturn foo() {
    cout << i++ << ". From coroutine\n";
    co_return;
}
void T2_Return() {
    auto ro_foo = foo();
    auto handle = ro_foo.handle;

    cout << i++ << ". Control return to Main\n";

    while(!handle.done()) {
        cout << i++ << ". resuming the coroutine again\n";
        handle();
    }
}


// ReturnObject for co_return

struct ReturnObjectReturn2 {
    struct promise_type {
        int val_;

        void return_value(int val ) {
            cout << i++ << ". promise_type::return_void\n";
        }

        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; } // Changed to suspend_always

        ReturnObjectReturn2 get_return_object() {
            return { std::coroutine_handle<promise_type>::from_promise(*this) };
        }
        void unhandled_exception() {}
    };

    std::coroutine_handle<promise_type> handle;
    ReturnObjectReturn2(std::coroutine_handle<promise_type> h) : handle{h} {}

    operator std::coroutine_handle<promise_type>() const { return handle; }
    
    int get_value() const { return handle.promise().val_; }

    ~ReturnObjectReturn2() {
        if (handle) handle.destroy(); // Destroy handle in destructor
    }
};
ReturnObjectReturn2 bar(int val) {
    cout << i++ << ". From coroutine\n";
    co_return val;
}
void T2_Return2() {
    auto ro_bar = bar(5);
    auto handle = ro_bar.handle;

    cout << i++ << ". Control return to Main\n";

    while(!handle.done()) {
        cout << i++ << ". resuming the coroutine again\n";
        handle();
    }
}

int main() {
    T();
    T2_Return();
    cout << "\n";
    T2_Return2();
    return 0;
}

/*  
0. Control return to Main
1. resuming the coroutine again
2. From coroutine
3. promise_type::return_void
4. resuming the coroutine again*/