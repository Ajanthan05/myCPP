#include <charconv> // for from_chars
#include <iostream>
#include <system_error> // for std::errc
#include <vector>

#include <sstream>
#include <optional>
#include <cstring> 
#include <expected>
#include <fstream>
// #include <tl/expected.hpp>

struct from_chars_result {
    const char* ptr;  // Pointer to the character *after* the last parsed character
    std::errc ec;     // Error code (no error if ec == std::errc{})
};
template <typename T>
from_chars_result from_chars(const char* first, const char* last, T& value, int base = 10);

/*  first / last: The range of characters to parse.
value: Reference to the variable to store the result.
base: Number base (default 10; valid: 2 to 36).


ptr: Points to the position in the input buffer right after the last character successfully parsed.

ec: Indicates the error status. std::errc{} means success. Other values might be:

std::errc::invalid_argument: No conversion could be performed.

std::errc::result_out_of_range: Value is out of range for the target type.



std::from_chars is low-level and locale-independent (unlike std::stringstream).

It works only for numeric conversions, not floating-point (use floating-point overloads in C++17 and later with extra support).

It’s a non-throwing API, designed for performance.
*/



void T() {
    const char* str = "12345";
    int value;

    auto result = std::from_chars(str, str + 5, value);

    if (result.ec == std::errc()) {
        std::cout << "Parsed value: " << value << '\n';
    } else {
        std::cout << "Failed to parse number\n";
    }
}

void T2() {
    const char* str1 = "abc";
    const char* str2 = "9999999999999999999999999999"; // overflows `int`

    int val;

    auto res1 = std::from_chars(str1, str1 + strlen(str1), val);
    auto res2 = std::from_chars(str2, str2 + strlen(str2), val);

    if (res1.ec == std::errc::invalid_argument)
        std::cout << "res1: invalid_argument\n";
    if (res2.ec == std::errc::result_out_of_range)
        std::cout << "res2: result_out_of_range\n";

    std::cout << "res1.ptr == str1? " << (res1.ptr == str1 ? "yes" : "no") << "\n";
    std::cout << "res2.ptr == str2? " << (res2.ptr == str2 ? "yes" : "no") << "\n";
}

/*  ptr == first and ec == std::errc::invalid_argument ⇒ nothing parsed (invalid input)

ptr > first and ec == std::errc::result_out_of_range ⇒ partial parse, but value overflowed

If you want to find the exact character that caused failure, you must manually inspect the input or parse character-by-character.

| Case                                | `ec`                             | `ptr` Value                | Notes                        |
| ----------------------------------- | -------------------------------- | -------------------------- | ---------------------------- |
| Empty string                        | `std::errc::invalid_argument`    | `first`                    | Nothing was parsed           |
| Non-numeric characters              | `std::errc::invalid_argument`    | `first`                    | First char was invalid       |
| Overflow (too large/small for type) | `std::errc::result_out_of_range` | points to last valid digit | Overflow after partial parse |
| Valid integer                       | `std::errc{}` (i.e., success)    | points past last digit     | Correct result               |
*/


enum class error { success = 0, bad_whole, no_decimal, bad_decimal };

std::error_code make_error_code(error e) noexcept {
    static const struct : std::error_category {
        virtual const char* name() const noexcept override {
            return "Decimal Parser";
        }

        virtual std::string message(int code) const override {
            switch (static_cast<error>(code)) {
                case error::success: return "Success";
                case error::bad_whole: return "Invalid whole part";
                case error::no_decimal: return "Missing decimal point";
                case error::bad_decimal: return "Invalid decimal part";
                default: return "Unknown";
            }
        }

        virtual std::error_condition default_error_condition(int code) const noexcept override {
            if (code != 0)
                return std::errc::invalid_argument;
            return {};
        }

        virtual bool equivalent(int code, const std::error_condition& condition) const noexcept override {
            return default_error_condition(code) == condition;
        }

        virtual bool equivalent(const std::error_code& code, int condition) const noexcept override {
            return (*this == code.category()) && (code.value() == condition);
        }
    } category;

    return std::error_code(static_cast<int>(e), category);
}
namespace std {
    template<>
    struct is_error_code_enum<error> : true_type {};
}


// ✅ Best Practices for Error Handling in C++
void Exception() {
    /*  1. Use Exceptions for Exceptional Cases
Prefer using exceptions (try-catch) to handle unexpected, unrecoverable errors (e.g., out of memory, file system corruption).
Avoid using exceptions for control flow or expected conditions.


try/catch blocks are translated to metadata tables at compile time. When throw is 
executed, the runtime creates an exception object, unwinds the stack, and jumps to 
the appropriate catch block. Destructors are called during unwinding to clean up 
resources. This model provides zero runtime cost unless an exception is actually 
thrown.

Behind the scenes:

Compiler generates metadata for func().

may_throw() executes.

If throw occurs:

Stack is unwound.

Destructors called.

Matching catch found.

Catch block executes.*/
    try {
        std::vector<int> v(1000000000); // May throw std::bad_alloc
    } catch (const std::bad_alloc& e) {
        std::cerr << "Memory allocation failed: " << e.what() << '\n';
    }
    

    /*  2. Avoid Exceptions in Performance-Critical Code (or use noexcept)
For real-time or low-latency code (e.g., embedded, games, drivers), exceptions can be disabled with -fno-exceptions and use error codes instead.

Mark noexcept on functions that are guaranteed not to throw.

3. Use RAII to Manage Resources
Leverage Resource Acquisition Is Initialization to manage memory, file handles, sockets, etc.

This avoids leaks in case of errors.*/
    std::ifstream file("data.txt");
    if (!file)
        throw std::runtime_error("Cannot open file");

    /*  4. Use std::optional, std::expected, or Error Codes for Simple Cases
For functions that may fail but aren't exceptional (e.g., parsing, validation), use return types like*/
    std::optional<int> parse_int(const std::string& s);
    std::expected<int, std::string> parse_and_validate(const std::string& s);   // run on 23
    // tl::expected<int, std::string> parse_and_validate(const std::string& s);

    // Or use error codes when exceptions are disabled:
    enum class error_code { success, parse_error, overflow };
    std::pair<int, error_code> parse_int(const char* s);


}

// Example Without RAII (Prone to Leaks)

// RAII + Exception Safety This ensures fclose() is called, even if an exception is thrown during file use.
class File {
    FILE* f;
public:
    File(const char* name) { f = fopen(name, "r"); }
    ~File() { if (f) fclose(f); }  // Always runs, even during exceptions
};

void risky_function() {
    FILE* fp = fopen("data.txt", "r");
    if (!fp) throw std::runtime_error("Failed to open file");

    // ... some code that may throw

    fclose(fp); // Not reached if exception thrown above — resource leak!
}

// ✅ Example With RAII (Safe)
#include <fstream>
void safe_function() {
    std::ifstream file("data.txt"); // opens file
    if (!file.is_open()) throw std::runtime_error("Failed to open file");

    // ... use file normally

} // file is automatically closed when it goes out of scope (destructor called)


/*  
std::exception
├── std::runtime_error
│   ├── std::overflow_error
│   └── std::underflow_error
├── std::logic_error
│   ├── std::invalid_argument
│   ├── std::domain_error
│   └── std::out_of_range

throw std::invalid_argument("bad input");
throw -1;               // int
throw MyException();    // user-defined class

Best Practices
| Practice                                  | Why                                                  |
| ----------------------------------------- | ---------------------------------------------------- |
| Catch exceptions **by const reference**   | Avoid slicing & improve performance                  |
| Use **exception-safe RAII**               | Destructors run automatically during stack unwinding |
| **Don't use exceptions for flow control** | They are for *errors*, not for logic                 |
| Use **custom exceptions** if needed       | More meaningful, structured handling                 |
| Always clean up resources in destructors  | Prevent memory/resource leaks                        |

C++17: std::uncaught_exceptions()
Lets you detect how many exceptions are currently being handled — useful for writing safe destructors.

🚨 Common Mistakes
❌ catch(...) without rethrow/log
❌ Resource leaks when not using RAII
❌ Ignoring exception safety in destructors
❌ Throwing raw pointers

Rethrowing Exceptions
try {
    throw;
} catch (...) {
    // handle
    throw;  // rethrow same exception
}
Use when you want to log or wrap the exception.

Custom Exception Example
class MyException : public std::exception {
public:
    const char* what() const noexcept override {
        return "My custom error";
    }
};
*/
// Let me know if you want examples for file I/O, network sockets, or system calls like pipe(), fork(), etc.
// noexcept
int main() {
    T();
    T2();
}
