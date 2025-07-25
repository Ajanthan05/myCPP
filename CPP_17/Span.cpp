#include <iostream>
#include <span>
#include <vector>
#include <cassert>
#include <array>

// using namespace std;

/* std::span is a lightweight view over a contiguous sequence of elements.
 It does not own the data — it just provides safe, bounds-checked access to it */

// template<std::size_t N>
// void func(std::span<int, N> s); // static extent

// void func(std::span<int> s); // dynamic extent
/*
| Span Type           | Description                                    |
| ------------------- | ---------------------------------------------- |
| `std::span<int, 5>` | Fixed-size span (extent known at compile time) |
| `std::span<int>`    | Dynamic-size span (size set at runtime)        |
*/

template<class T, std::size_t Extent = std::dynamic_extent>
class SpaN;
/* Extent: Number of elements (std::dynamic_extent for runtime-sized span).
If Extent == std::dynamic_extent (i.e., -1), then size is stored at runtime.
If Extent is any other value, then the span's size is fixed at compile time.
std::dynamic_extent Explained +> constexpr std::size_t dynamic_extent = -1;

constexpr std::size_t dynamic_extent = static_cast<std::size_t>(-1);
    static_cast<std::size_t>(-1) gives the largest possible std::size_t value, i.e., 18446744073709551615 on 64-bit systems.
    This value acts as a sentinel constant, named dynamic_extent.
*/


constexpr std::size_t dynamic_extent = static_cast<std::size_t>(-1);

template <typename T, std::size_t Extent = dynamic_extent>
class span;

// -----------------------------
// Fixed extent version
// -----------------------------
template <typename T, std::size_t Extent>
class span {
    T* ptr_;

public:
    using element_type = T;
    using value_type = std::remove_cv_t<T>;
    using size_type = std::size_t;
    static constexpr size_type extent = Extent;

    constexpr span(T* ptr) : ptr_(ptr) { }

    constexpr T* data() const noexcept { return ptr_; }
    constexpr size_type size() const noexcept { return Extent; }
    constexpr bool empty() const noexcept { return Extent == 0; }

    constexpr T& operator[](size_type idx) const {
        assert(idx < Extent);
        return ptr_[idx];
    }

    constexpr span<T, Extent> subspan(size_type offset) const {
        assert(offset <= Extent);
        return span<T, Extent - offset>(ptr_ + offset);
    }
};

// -----------------------------
// Dynamic extent version
// -----------------------------
template <typename T>
class span<T, dynamic_extent> {
    T* ptr_;
    std::size_t size_;

public:
    using element_type = T;
    using value_type = std::remove_cv_t<T>;
    using size_type = std::size_t;
    static constexpr size_type extent = dynamic_extent;

    constexpr span() : ptr_(nullptr), size_(0) {}
    constexpr span(T* ptr, std::size_t size) : ptr_(ptr), size_(size) {}

    template <std::size_t N>
    constexpr span(T (&arr)[N]) : ptr_(arr), size_(N) {}

    constexpr T* data() const noexcept { return ptr_; }
    constexpr size_type size() const noexcept { return size_; }
    constexpr bool empty() const noexcept { return size_ == 0; }

    constexpr T& operator[](size_type idx) const {
        assert(idx < size_);
        return ptr_[idx];
    }

    constexpr span<T> subspan(size_type offset, size_type count = dynamic_extent) const {
        assert(offset <= size_);
        size_type remaining = size_ - offset;
        return span<T>(ptr_ + offset, count == dynamic_extent ? remaining : count);
    }
};

template <typename Span>
void print_info(const Span& s) {
    using T = typename Span::value_type;
    std::cout << "Span of type: " << typeid(T).name() << "\n";
    std::cout << "Size: " << s.size() << "\n";
    
    if constexpr (Span::extent != dynamic_extent)
        std::cout << "Extent known at compile time\n";
    else
        std::cout << "Extent is dynamic\n";
}

void T_Span() {
    int arr[] = {1, 2, 3, 4, 5};

    // Fixed extent
    span<int, 5> s_fixed(arr);
    std::cout << "Fixed: ";
    for (size_t i = 0; i < s_fixed.size(); ++i)
        std::cout << s_fixed[i] << " ";
    std::cout << "\n";

    // Dynamic extent
    span<int> s_dyn(arr, 5);
    std::cout << "Dynamic: ";
    for (size_t i = 0; i < s_dyn.size(); ++i)
        std::cout << s_dyn[i] << " ";
    std::cout << "\n";

    // Subspan
    auto s_sub = s_dyn.subspan(2, 2);
    std::cout << "Subspan: ";
    for (size_t i = 0; i < s_sub.size(); ++i)
        std::cout << s_sub[i] << " ";
    std::cout << "\n";
}




template <typename T>
class span2 {
public:
    // Default constructor
    constexpr span2() noexcept : data_{nullptr}, length{0} {}

    // From pointer and length
    constexpr span2(T* start, std::size_t len) noexcept
        : data_{start}, length{len} {}

    // From C-style array
    template <std::size_t N>
    constexpr span2(T (&arr)[N]) noexcept : span2(arr, N) {}

    // From const C-style array
    template <std::size_t N>
    constexpr span2(const T (&arr)[N]) noexcept : span2(arr, N) {}

    // From std::array
    template <std::size_t N, typename AT = std::remove_const_t<T>>
    constexpr span2(std::array<AT, N>& arr) noexcept
        : span2(arr.data(), arr.size()) {}

    // From const std::array
    template <std::size_t N>
    constexpr span2(const std::array<std::remove_const_t<T>, N>& arr) noexcept
        : span2(arr.data(), arr.size()) {}

    // Size of span2
    constexpr std::size_t size() const noexcept { return length; }

    // Element access
    constexpr T& operator[](std::size_t i) const noexcept { return data_[i]; }

    // Data pointer
    constexpr T* data() const noexcept { return data_; }

    // Check if empty
    constexpr bool empty() const noexcept { return length == 0 || data_ == nullptr; }

    // Iterators
    constexpr T* begin() const noexcept { return data_; }
    constexpr T* end() const noexcept { return data_ + length; }

private:
    T* data_;
    std::size_t length;
};





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
Static extent: The size is known at compile time.
→ std::span<int, 5>
The static version tells the compiler, "This span will always be exactly 5 elements long." 
That can enable:
Slight performance gains (no runtime size tracking)
Safer code (compiler checks size for you)

Dynamic extent: Size known at runtime (e.g. std::span<int>)
Dynamic extent: The size is determined at runtime.
→ std::span<int>

*/


    // Dynamic extent
std::span<int> s_dynamic(arr, 5);

// Static extent
std::span<int, 3> s_static(arr); // Size must match exactly!

    /*  Common Methods
| Function                 | Description                      |
| ------------------------ | -------------------------------- |
| `size()`                 | Number of elements               |
| `empty()`                | Checks if span is empty          |
| `data()`                 | Pointer to first element         |
| `operator[]`             | Element access (unchecked)       |
| `front()`, `back()`      | First and last elements          |
| `begin()`, `end()`       | Iterators                        |
| `subspan(offset, count)` | Create subview                   |
| `first(n)`, `last(n)`    | Subviews for first/last elements |
*/

    /*  Limitations
    Must point to contiguous memory.

    Not usable with std::list, std::map, etc.

    Doesn't work with dynamically generated or disjoint memory layouts.

    No memory ownership = no lifetime guarantee! Don’t return spans to temporary data.
*/

/*  EDGE CASES
int arr[] = {1, 2, 3, 4, 5};
Passing a C-style array directly
print(arr); // OK! Size is deduced automatically
In this case, the compiler knows the size of the array at compile time, so it can automatically create a std::span<int> of the correct size.

❌ Case 2: Passing a pointer to the array
int* ptr = arr;
print(ptr); // ❌ Won't compile
This won't work because int* doesn't carry size information — and std::span<int> needs both a pointer and a size.
print(std::span<int>(ptr, size));


*/


    T_Span();

    // Let me know if you want examples of using std::span with std::ranges, constexpr, or fixed-extents specifically.
}

/*Great question! Let's break this down and explain what each of these typedefs and constants mean — and whether they're used internally (e.g. for iteration, bounds, traits, etc.).

---

## 🔍 Line-by-line Breakdown

### \`\`\`cpp

using element\_type = T;

````
- This is an **alias** for the type of the elements the span refers to.
- It keeps the interface generic and consistent.
- Used in generic programming — helps other code extract the base type.

✅ **Purpose:** Metadata. Useful in templated code or tools (like ranges or views).

---

### ```cpp
using value_type = std::remove_cv_t<T>;
````

* This removes `const` or `volatile` qualifiers from `T`.

  * E.g. if `T = const int`, then `value_type = int`
* Follows the STL convention: `value_type` is the plain type stored.

✅ **Purpose:** For traits, algorithms, or containers that require the unqualified type (e.g. copying, sorting, etc.)

---

### \`\`\`cpp

using size\_type = std::size\_t;

````
- Defines a type alias for indexing or sizing operations.
- Makes the code more portable and consistent with STL containers.

✅ **Purpose:** Used internally — for `size()`, `operator[]`, loops, etc.

---

### ```cpp
static constexpr size_type extent = Extent;
````

* This exposes the `Extent` template parameter as a constant.
* For fixed-size spans, this will be a compile-time constant.
* For dynamic-size spans, this will equal `dynamic_extent`.

✅ **Purpose:** Allows you (or meta-programming code) to check at compile time:

```cpp
if constexpr (span<int, 5>::extent != std::dynamic_extent) {
    // known size at compile time
}
```

---

## 🔄 Are These Used for Iteration?

Not directly — **iteration typically uses `begin()` and `end()`**, which return raw pointers or custom iterators. But:

| Name           | Used for iteration?   | Notes                                                    |
| -------------- | --------------------- | -------------------------------------------------------- |
| `element_type` | ❌                     | Mostly metadata, useful in templates                     |
| `value_type`   | ❌                     | Used in traits or STL algorithms                         |
| `size_type`    | ✅                     | Used internally for `size()`, `operator[]`, loop indices |
| `extent`       | ✅ (in specialization) | Enables optimizations when size is known at compile time |

---

## ✅ Example of Usage

```cpp
template <typename Span>
void print_info(const Span& s) {
    using T = typename Span::value_type;
    std::cout << "Span of type: " << typeid(T).name() << "\n";
    std::cout << "Size: " << s.size() << "\n";
    
    if constexpr (Span::extent != dynamic_extent)
        std::cout << "Extent known at compile time\n";
    else
        std::cout << "Extent is dynamic\n";
}
```

This works for both fixed and dynamic spans using those typedefs.

---

## 🧠 Summary

| Alias / Constant | Purpose                                  | Internally used? |
| ---------------- | ---------------------------------------- | ---------------- |
| `element_type`   | Exposes original type `T`                | No               |
| `value_type`     | Stripped version for use in algorithms   | No               |
| `size_type`      | Used for indexing/sizing (`std::size_t`) | ✅ Yes            |
| `extent`         | Holds the fixed size or dynamic extent   | ✅ Sometimes      |

Let me know if you'd like to add `begin()/end()` methods or understand how `std::span` interacts with `std::ranges`.
*/