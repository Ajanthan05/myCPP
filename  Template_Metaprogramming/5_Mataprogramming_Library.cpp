#include<iostream>
#include<type_traits>
#include<string>
#include<list>
#include<tuple>

template<bool condition, typename THEN, typename ELSE>
struct if_;

template<typename THEN, typename ELSE>
struct if_<true, THEN, ELSE>{
    using type = std::true_type;
};

template<typename THEN, typename ELSE>
struct if_<false, THEN, ELSE>{
    using type = std::false_type;
};

bool contains(const std::string& search, std::list<std::string> l) {
    if(l.empty()) return false;
    else {
        if (search==l.front()) return true;
        else {
            l.pop_front();
            return contains(search, l);
        }
    }
}

void T(){
    std::list<std::string> list{"int", "float", "double"};
    std::cout <<std::boolalpha;
    std::cout << contains("bool", list) << "\n";
}


template<typename...>
struct type_list{};

template<typename LIST>
struct empty : std::false_type{};

template<>
struct empty< type_list<> > : std::true_type{};

static_assert(empty< type_list<> >::value);
static_assert(empty< type_list<float> >::value == false);

template<typename LIST>
struct front{};

template<typename T0, typename... T1toN>
struct front<type_list<T0, T1toN...>>{
    using type = T0;
};

static_assert(std::is_same_v<front< type_list<int, bool> >::type, int>);

template<typename LIST>
struct pop_front;

template<typename T0, typename... T1toN>
struct pop_front<type_list<T0, T1toN...>>{
    // return the new list by defining the type member alias
    using type = type_list<T1toN...>;
};

static_assert(std::is_same_v< pop_front<type_list<int, bool, float>>::type, type_list<bool, float> >);

template<typename LIST>
using front_t = typename front<LIST>::type;

template<typename LIST>
using pop_front_t = typename pop_front<LIST>::type;

// template variable for empty meta function
template<typename LIST>
static constexpr bool empty_v = empty<LIST>::value;

template<typename SEARCH, typename LIST>
struct contains_type;

template<typename SEARCH, typename LIST>
struct non_empty_contains_type:
         if_< // IF
            std::is_same_v<SEARCH, front_t<LIST>>,
            // THEN
            std::true_type,
            // ELSE
            contains_type<SEARCH, pop_front_t<LIST> >
        >::type
{};

template<typename SEARCH, typename LIST>
struct contains_type :
    if_< // If
        empty_v<LIST>,
        // THEN
        std::false_type,
        // ELSE
        non_empty_contains_type<SEARCH, LIST>
    >::type
{};


int main() {
    T();
    type_list<int, float, double, bool, int> types;
    std::cout << contains_type<bool, decltype(types)>::value << "\n";
    T();    
}