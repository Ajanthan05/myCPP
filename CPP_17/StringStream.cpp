#include <sstream>
#include <iostream>
#include <string>

/*
std::stringstream      // Read and write
std::istringstream     // Read-only
std::ostringstream     // Write-only

| Function            | Description                           |
| ------------------- | ------------------------------------- |
| `str()`             | Get/set the internal string           |
| `str("new string")` | Sets new content                      |
| `clear()`           | Clears error state (useful after EOF) |
| `<<`                | Write to stringstream                 |
| `>>`                | Read from stringstream                |
| `eof()`             | True if end-of-stream reached         |
| `good()`            | True if no errors occurred            |
| `fail()`            | True if a read/write failed           |

Resetting a stringstream
ss.str("");       // Clear contents
ss.clear();       // Clear error flags (like failbit or eofbit)

| Feature             | `stringstream`        | `from_chars`     | `stoi` / `stod` |
| ------------------- | --------------------- | ---------------- | --------------- |
| Locale-aware        | ✅ Yes                 | ❌ No             | ✅ Yes           |
| Throws exceptions   | ❌ No (check `fail()`) | ❌ No             | ✅ Yes           |
| Header              | `<sstream>`           | `<charconv>`     | `<string>`      |
| Performance         | ❌ Slower              | ✅ Very fast      | ✅ Medium        |
| Read multiple types | ✅ Yes (`>>`)          | ❌ No (single op) | ❌ No            |

*/
int main() {
    std::stringstream ss("42 hello 3.14");
    
    int i;
    std::string word;
    float f;

    ss >> i >> word >> f;

    std::cout << "int: " << i << "\nstring: " << word << "\nfloat: " << f << "\n";

    ss.clear();
    ss << "The result is " << 42 << " and pi is " << 3.14;
    std::string result = ss.str();
    std::cout << result;  // Output: The result is 42 and pi is 3.14


    // String to Number
    std::string input = "123";
    int num;
    std::stringstream(input) >> num;

    // Number to String
    int num = 123;
    std::string str;
    std::stringstream ss1;
    ss1 << num;
    str = ss1.str();

}
