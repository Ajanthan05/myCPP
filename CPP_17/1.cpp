#include <iostream>
#include <vector>
#include <climits>
#include <ctime>

int main() {

    // If starement with initializer
    
/* ajanthan@AjantharajaH:~/myCpp/CPP_17$ g++ 1.cpp && ./a.out
odd: 1804289383
ajanthan@AjantharajaH:~/myCpp/CPP_17$ g++ 1.cpp && ./a.out
odd: 1804289383
ajanthan@AjantharajaH:~/myCpp/CPP_17$ g++ 1.cpp && ./a.out
odd: 1804289383
ajanthan@AjantharajaH:~/myCpp/CPP_17$ g++ 1.cpp && ./a.out
odd: 1804289383
*/

// To avoid same result
    std::srand(std::time(nullptr));
    
    if (int random = std::rand(); random % 2 == 0) {
        std::cout << "even: " << random << "\n";
    }
    else {
        std::cout << "odd: " << random << "\n";
    }
    // std::cout << "If: " << random << "\n";

    return 0;
}