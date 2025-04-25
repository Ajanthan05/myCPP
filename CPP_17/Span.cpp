#include <iostream>
#include <span>
#include <vector>
#include <array>

using namespace std;

/* std::span is a lightweight view over a contiguous sequence of elements. It does not own the data — it just provides safe, bounds-checked access to it

*/

template<std::size_t N>
void func(std::span<int, N> s); // static extent

void func(std::span<int> s); // dynamic extent

void print(std::span<int> data) {
    for (int x : data) {
        std::cout << x << ' ';
    }
    std::cout << '\n';
}

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    print(vec); // works directly!

    int arr[] = {10, 20, 30};
    print(arr); // also works

    std::array<int, 3> stdarr = {100, 200, 300};
    print(stdarr); // works too

    /*  Key Features
    ✅ 1. Non-owning
    span doesn’t allocate or free memory — it just refers to existing memory.

    ✅ 2. Safe bounds-checking (in debug)
    Offers array-style access (operator[]) and range-based iteration.

    In debug builds, may throw out-of-bounds exceptions.

    ✅ 3. View into any contiguous memory:
    Works with:

    C-style arrays

    std::array

    std::vector

    std::string_view (via span<const char> or span<char>)
    */

    // Slicing support :- You can easily create sub-views (like Python slices)
    std::span<int> s1 = vec;
    auto mid1 = s1.subspan(1, 3); // 2 3 4 
    print(mid1);

    // Also supports fixed-size spans:
    std::span<int, 3> s(arr); // static extent

    /*   Static vs Dynamic Extent
Static extent: Size known at compile time (e.g. std::span<int, 5>)

Dynamic extent: Size known at runtime (e.g. std::span<int>)*/


    // Dynamic extent
std::span<int> s_dynamic(arr, 5);

// Static extent
std::span<int, 3> s_static(arr); // Size must match exactly!

    /*  Common Methods
    s.size();       // number of elements
    s.empty();      // check if span is empty
    s.front();      // first element
    s.back();       // last element
    s.data();       // pointer to underlying data
    s.subspan(pos, len); // slice view
    */

    /*  Limitations
    Must point to contiguous memory.

    Not usable with std::list, std::map, etc.

    Doesn't work with dynamically generated or disjoint memory layouts.

    No memory ownership = no lifetime guarantee! Don’t return spans to temporary data.
*/
}
