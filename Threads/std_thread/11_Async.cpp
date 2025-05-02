#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>

#include <condition_variable>
using namespace std;
#include <future>

/*  std::async is a C++ Standard Library function that runs a function asynchronously, meaning:
The function starts executing in parallel (in another thread), or lazy (only when needed).
It returns a std::future to get the result later.
.get()	Blocks (waits) until the task is done, then returns the result.
.valid()	Checks if the future still holds a value.

📌 Important Notes
std::async may or may not spawn a new thread — unless you force std::launch::async.

Future must be .get() or .wait() eventually — otherwise destructor may block.

std::future is move-only (cannot copy it).

Once you .get(), the future becomes invalid.
*/
int slow_add(int a, int b) {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return a + b;
}

void test() {
    std::future<int> result = std::async(slow_add, 3, 4);

    std::cout << "Doing other work...\n";

    int value = result.get();  // waits if not ready
    std::cout << "Result is " << value << std::endl;
    /*  What happens:
slow_add(3, 4) runs asynchronously.
Meanwhile, main() can continue doing other stuff.
Later, .get() waits for the result if it's not ready.*/
}

/*  ⚡ Launch Policy
std::async has an optional launch policy:
std::future<int> result = std::async(std::launch::async, slow_add, 3, 4);
There are 3 options:
1)  std::launch::async	Force to run in new thread immediately.
2)  std::launch::deferred	Delay execution — only runs when .get() or .wait() is called.
3)  Default (unspecified)	Compiler chooses async or deferred based on optimization.
👉 If you want real parallelism, always specify std::launch::async.*/

void PassingLambda() {
    std::future<int> result = std::async(std::launch::async, [] () {
        cout << "Ajan\n";
        return 5;
    });
    cout << result.get() << endl;
}

/*  Exception Handling
If the function throws an exception, .get() will rethrow it.*/
void Exception() {
    /*  throw and never return, the compiler doesn't know what type you intend.
So you must explicitly declare the return type (-> int) to tell the compiler:*/
    auto f = std::async([]() -> int {
        throw std::runtime_error("Oops!");
    });
    
    try {
        int result = f.get();
    } catch (const std::exception& e) {
        std::cout << "Caught exception: " << e.what() << '\n';
    }
    
}

/*  🎯 When to use deferred (lazy async)?
You want optional expensive computation (maybe you won't need the result).

You want more control: start later manually.

You want serial execution with a future interface (sometimes good for testing).

🚨 Important catch
If you use deferred, and never call .get() or .wait(),
then the function never runs!

That's why in production code, be careful when you use deferred.
You must be sure you eventually consume the future.*/


int main() {
    PassingLambda();
    Exception();
    return 0;
} 