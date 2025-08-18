#include <iostream>

using namespace std;


class Singleton {
public:
    static Singleton* instance()
    {
    static Singleton* instance_ = NULL;
    if(!instance_)
    instance_ = new Singleton();
    return instance_;
    }
    void func(...);

private:
    Singleton();
    Singleton(const Singleton&);
    void operator=(const Singleton&);
};

Singleton::instance()->func(...);

/*  
Allocates memory on the heap via new.

You must manually delete it (which is missing here) or accept a memory leak.

Not thread-safe by default.

If two threads call instance() simultaneously on first access, double allocation may occur.

Requires explicit locking for thread safety (or use std::call_once in modern C++).
Heap allocation (new) lives until program ends (unless deleted).

But no destructor call shown → potential memory leak.

You’d need something like atexit() or manual delete.

*/

/*  ❌ 1. Acts as Hidden Dependencies
Functions that use a Singleton don’t explicitly show that they rely on it:

cpp
Copy
Edit
void doWork() {
    auto val = Random_Imp::Float();  // uses a hidden global
}
Problem: You can’t tell by looking at doWork()'s signature that it depends on Random_Imp.

Consequence: Makes code harder to reason about, test, and reuse.

✅ Better: Use explicit dependency injection (pass as parameter).

❌ 2. No Dependency Injection for Testing
Singletons are hard to replace with mocks or stubs in unit tests.

You can’t inject a fake Random_Imp — it’s hardwired:

cpp
Copy
Edit
EXPECT_EQ(Random_Imp::Float(), 0.5f);
Makes unit tests:

Less isolated

Harder to configure

Often flaky or require ugly workarounds (e.g., hooks, global overrides)

✅ Better: Use interfaces and dependency injection frameworks.

❌ 3. Initialization is Out of Your Control
You can’t control when or how a Singleton is created.

Created the first time it's accessed — may be too late or too early.

Causes order-of-initialization bugs, especially with multiple static Singletons across files.

✅ Better: Explicit lifecycle control through application startup/init code.

❌ 4. Multiple Runs Can Yield Different Results
This is a problem if Singleton holds internal state.

Example:

cpp
Copy
Edit
class Logger {
    int messageCount = 0;
    void log(const std::string& msg) { messageCount++; }
};
In test suite, Singleton may retain state between tests unless explicitly reset.

In apps, race conditions may occur if accessed from multiple threads.

✅ Better: Use stateless or short-lived objects.

❌ 5. Initialization Order of Multiple Singletons
If Singletons depend on each other:

cpp
Copy
Edit
static Logger logger;
static Config config;  // logger used in config's constructor
If logger isn’t initialized before config, it’s undefined behavior.

Fixing it requires fragile manual workarounds.

✅ Better: Centralized initialization code.

❌ 6. State Tied to Program Lifetime
Singletons exist from first use until program ends.

If you need to restart, reset, or change them — tough luck.

Order of function calls becomes critical: you must call methods in the right sequence.

✅ Better: Encapsulate state in objects with well-defined lifetimes.

⚠️ Summary of Drawbacks
Issue	Consequence
Hidden dependencies	Harder to understand and maintain
No test injection	Hard to mock/test in isolation
No lifecycle control	Bugs from initialization order or timing
Retained state	Inconsistent behavior, flaky tests
Global coupling	Fragile setup; tight dependencies
Program-tied state	Cannot restart/reset cleanly

🟢 When Singleton Is Still Okay
Global configuration

Logging utilities

Performance-critical, thread-safe access (with no state)

Framework code (well-documented usage)

But for most business logic or testable components — avoid it.

https://www.youtube.com/watch?v=K5c7uvWe_hw*/

/* Mayers Singleton which avoid certain issues with global objects, namely
the static initialization order fiasco
c++11 guarantees the initialization happens in a thread safe manner the first time 
the function is called
*/