// file: offset_ptr_example.cpp
#include <type_traits>
#include <utility>
#include <cstdint>
#include <iostream>

// Helper: is_static_castable (expression SFINAE)
template<class From, class To, class = void>
struct is_static_castable : std::false_type {};

template<class From, class To>
struct is_static_castable<From, To,
    std::void_t<decltype(static_cast<To>(std::declval<From>()))>> : std::true_type {};

template<class From, class To>
inline constexpr bool is_static_castable_v = is_static_castable<From, To>::value;


// A tiny offset_ptr toy
template<class T>
class offset_ptr {
    std::uintptr_t m_ptr; // offset stored as integer
public:
    // normal ctor from raw pointer
    explicit offset_ptr(T* p) {
        m_ptr = reinterpret_cast<std::uintptr_t>(p) - reinterpret_cast<std::uintptr_t>(this);
    }

    // get the actual pointer
    T* ptr() const noexcept {
        return reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + m_ptr);
    }

    // 1) implicit converting ctor when U* -> T* is implicitly convertible
    template<class U,
             std::enable_if_t<std::is_convertible_v<U*, T*>, int> = 0>
    offset_ptr(const offset_ptr<U>& rhs) : offset_ptr(rhs.ptr()) {}

    // 2) explicit converting ctor when U* is static_castable to T* but NOT implicitly convertible
    template<class U,
             std::enable_if_t<!std::is_convertible_v<U*, T*> &&
                              is_static_castable_v<U*, T*>, int> = 0>
    explicit offset_ptr(const offset_ptr<U>& rhs) : offset_ptr(static_cast<T*>(rhs.ptr())) {}

    // default copy/move ctors are fine
};


// test types
struct Base { virtual ~Base() = default; };
struct Derived : Base { };

int main() {
    Derived d;
    offset_ptr<Derived> opd(&d);

    // implicit convert Derived* -> Base*
    offset_ptr<Base> opb = opd; // uses implicit constructor
    std::cout << "implicit ok\n";

    // reverse conversion requires explicit ctor (downcast)
    // offset_ptr<Derived> opd2 = opb; // <- would be ill-formed (explicit)
    offset_ptr<Derived> opd2(opb); // direct-initialization allows explicit constructor
    std::cout << "explicit ok\n";

    return 0;
}
