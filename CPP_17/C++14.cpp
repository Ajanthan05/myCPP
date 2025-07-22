#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <bitset>

using namespace std;

/*  DIGIT SEPRATOR
Now there is better code readability with Digit Separator
long decval = 1'048'569
long decval = 10'48'569
*/

/*  BINARY LITERALS: now we can directly write it*/
void BINARY_LITERALS() {
    int b1 = 0xFF;  // Binary 11111111
    cout << b1 << "\n";

    int b2 = 0b11111111;    // 0b OR 0B
    int b3 = 0b1111'1111;

    cout << b2 << " " << b3 << "\n";

    // std::bitset<8> prints the binary form of 8 bits.
    cout << std::bitset<16>(b3) << "\n";

/*  
| Type       | Bits | Max Decimal Value | Max Hexadecimal | Max Binary (prefix `0b`)       |
| ---------- | ---- | ----------------- | --------------- | ------------------------------ |
| `uint8_t`  | 8    | 255               | `0xFF`          | `0b11111111`                   |
| `int8_t`   | 8    | 127               | `0x7F`          | `0b01111111`                   |
| `uint16_t` | 16   | 65,535            | `0xFFFF`        | `0b1111111111111111`           |
| `int16_t`  | 16   | 32,767            | `0x7FFF`        | `0b0111111111111111`           |
| `uint32_t` | 32   | 4,294,967,295     | `0xFFFFFFFF`    | `0b111...111` (32 ones)        |
| `int32_t`  | 32   | 2,147,483,647     | `0x7FFFFFFF`    | `0b011...111` (31 ones + sign) |

int max_int = 0x7FFFFFFF;      // = 2,147,483,647
int min_int = 0x80000000;      // = -2,147,483,648 (as signed)

*/
}

/*  [[deprecated]]
1. Deprecated means use of the name or entity declarred with this attribute is allowed, 
but descrage for some reason.
    2. Compiler gives warnings and if string literals are provided, they are included
in the warning 

    LIST WHAT CAN BE DEPRECATED
class, struct, union
typedef
variable
non-static data member
function 
nemespace
enumeration
enumerator
template specilation

** All modern compilers support this since C++14. Namespace deprecation requires C++20.
*/
[[deprecated("This function will be replaced with template add function")]]
int add(int a, int b) { return a+b; }


[[deprecated("Use NewStruct instead")]]
struct OldStruct {
    int a;
};

[[deprecated("Use NewUnion instead")]]
union OldUnion {
    int x;
    float y;
};
/* Below also correct
struct [[deprecated]] OldStruct {};

union [[deprecated]] OldUnion {
    int x;
    float y;
};
*/

// 2. Typedef
[[deprecated("Use std::string instead")]]
typedef char* CStyleString;

// Enumerator (enum value)
enum Status {
    OK,
    [[deprecated("Use Status::FAIL instead")]]
    ERROR
};

// Template Specialization
template<typename T>
struct Wrapper {
    void show() {}
};
template<>
struct [[deprecated("Specialization for int is deprecated")]] Wrapper<int> {
    void show() {}
};




    // VARIABLE TEMPLATE
template <typename T>
T pi = T(3.141'592'653'5897932384626433L);

void VARIABLE_TEMPLATE() {
    // Also
    pi<char> 'a';
    cout.precision(std::numeric_limits<long double>::max_degits10);
    cout << pi<int> << "\n";
    cout << pi<float> << "\n";
    cout << pi<double> << "\n";
    cout << pi<long double> << "\n";
    cout << pi<char> << "\n";

}




int main() {
    BINARY_LITERALS();

    return 0;
}