#include<iostream>
#include<type_traits>
#include<string>
#include<tuple>

/*
// for empty case printn();
void printn() {
    std::cout << "\n";
}

template <typename LAST>
void printn(LAST t) {
    std::cout << t << "\n";
}
// recurise templet base case has to define above
template <typename T0, typename... T1toN>
void printn(T0 t, T1toN... rest) {
    std::cout << t << ", ";
    printn(rest...);
}


template <typename TUPLE, std::size_t... indices>
void print_tuple_impl(TUPLE t, std::index_sequence<indices...>) {
    printn(std::get<indices>(t)...);
}


template <typename TUPLE>
void print_tuple(TUPLE t) {
    print_tuple_impl(t, std::make_index_sequence< std::tuple_size<TUPLE>::value >{} );
}

int main(){
    std::cout << std::boolalpha;
    printn(5, "Ajan", true);
    printn();

    auto tuple = std::make_tuple(5, "Ajan", true);
    print_tuple(tuple);
}
*/

////////////////     AFTER PERFECT FORWArding  /////////////////

void printn() {
    std::cout << "\n";
}

template <typename LAST>
void printn(LAST&& t) {
    std::cout << std::forward<LAST>(t) << "\n";
}
// recurise templet base case has to define above
template <typename T0, typename... T1toN>
void printn(T0&& t, T1toN&&... rest) {
    std::cout << std::forward<T0>(t) << ", ";
    printn(std::forward<T1toN>(rest)...);
}


template <typename TUPLE, std::size_t... indices>
void print_tuple_impl(TUPLE&& t, std::index_sequence<indices...>) {
    printn(std::get<indices>(std::forward<TUPLE>(t))...);
}


template <typename TUPLE>
void print_tuple(TUPLE&& t) {
    // print_tuple_impl(std::forward<TUPLE>(t), std::make_index_sequence< std::tuple_size<TUPLE>::value >{} );
    // We cant use tupple size in a reference
    print_tuple_impl(std::forward<TUPLE>(t), std::make_index_sequence<std::tuple_size< std::remove_reference_t< TUPLE> >::value >{} );
}

int main(){
    std::cout << std::boolalpha;
    printn(5, "Ajan", true);
    printn();

    auto tuple = std::make_tuple(5, "Ajan", true);
    print_tuple(tuple);
}