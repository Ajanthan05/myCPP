#include <iostream>
#include <cstring>
using namespace std;

class String {
private:
    char* res;
    unsigned int len;

public:
    String() : res(nullptr), len(0) {
        cout << "Default Constructor\n";
    }

    // Parameterized constructor
    String(const char* ch) {
        cout << "Parameterized Constructor\n";
        len = strlen(ch);
        res = new char[len + 1];
        strcpy(res, ch);
    }

    // Copy constructor
    // String(const String& str) {
    //     cout << "Copy Constructor\n";
    //     len = str.len;
    //     res = new char[len + 1];
    //     strcpy(res, str.res);
    // }

    /*  TO AVOID
    Program received signal SIGSEGV, Segmentation fault.
    __strcpy_avx2 () at ../sysdeps/x86_64/multiarch/strcpy-avx2.S:304
    */
    // Copy Constructor (Prevent copying empty String)
    String(const String& str) {
        if (str.res == nullptr || str.len == 0) {
            std::cout << "Copy Constructor - Skipped (Empty String)\n";
            res = nullptr;
            len = 0;
            return;
        }
        std::cout << "Copy Constructor\n";
        len = str.len;
        res = new char[len + 1];
        std::strcpy(res, str.res);
    }

    // Move constructor
    // && it's for rvalue
    // String(String&& str) noexcept {
    //     // cout << "Move Constructor\n";
    //     len = str.len;
    //     res = str.res;
    //     str.res = nullptr;
    //     str.len = 0;
    // }

    // Move Constructor (Fixed)
    String(String&& str) noexcept : res(str.res), len(str.len) {
        std::cout << "Move Constructor\n";
        str.res = nullptr;
        str.len = 0;
    }

    // Copy-and-Swap Assignment (CAS) Operator (Handles both Copy and Move Assignment)
    String& operator=(String str) { // Whitout & it will call copy constructor => str will store coppy of str2 when "str3 = str2;" is called
        cout << "Copy-and-Swap Assignment Operator\n";
        swap(*this, str);
        return *this;
    }

    // Swap function
    friend void swap(String& str1, String& str2) noexcept {
        cout << "Swap function\n";
        std::swap(str1.res, str2.res);
        std::swap(str1.len, str2.len);
    }

    unsigned int length() const {
        return len;
    }

    // Friend function for output stream overloading
    friend ostream& operator<<(ostream& out, const String& str) {
        cout << "Friend function for output stream overloading\n";
        if (str.res) out << str.res;
        return out;
    }

    // Friend function for input stream overloading
    friend istream& operator>>(istream& in, String& str) {
        cout << "Friend function for input stream overloading\n";
        char buffer[1000]; // Fixed-size buffer
        in >> buffer;
        delete[] str.res; // Free old memory
        str.len = strlen(buffer);
        str.res = new char[str.len + 1];
        strcpy(str.res, buffer);
        return in;
    }

    ~String() {
        cout << "Distructor\n";
        delete[] res;
    }
};

int main() {
    String str1; // Default constructor
    String str2 = "hello"; // Parameterized constructor
    String str3 = str2;  // Copy constructor
    str3 = str2; // No ambiguity now, copy-and-swap assignment operator

    // cout << str2 << endl; // Overloaded << operator
    // cin >> str1; // Overloaded >> operator
    // cout << str1 << endl;

    String str4(str1); // Copy constructor

    cout << "You entered: " << str1 << endl;

    String str5 = std::move(str2);
    cout << "After Move: " << str5 << endl;

    return 0;
}

