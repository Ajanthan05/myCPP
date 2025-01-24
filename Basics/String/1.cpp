#include <iostream>
#include <string>

/* Important Notes:
Both operands for the + operator must be strings or one can be a const char* (string literal).
You cannot directly add a char and a std::string using +. To concatenate a char, you need to convert it to a std::string.

*/

int main() {
    std::string str = "Number: ";
    char digit = '7';
    str = str + digit; // Convert 'digit' to string and concatenate
    std::cout << str << std::endl; // Output: Number: 7

    // std::string(size_t count, char ch) expects a single character (char) as the second argument
    str = str + std::string(10, '5');  // Convert char to string explicitly
    std::cout << str << std::endl; // Output: Number: 7

/* Use + when creating new strings from existing ones.
Use append() when modifying the original string directly.*/
    std::string str1 = "Hello, ";
    std::string str2 = "World!";
    str1.append(str2); // Appends str2 to str1
    std::cout << str1 << std::endl; // Output: Hello, World!

    return 0;
}