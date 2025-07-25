// Online C++ compiler to run C++ program online
#include <iostream>


int main() {
    int i = 12345;
    int ans=0;
    while(i) {
        int tmp = i%10;
        ans = ans*10 + tmp;
        i = i/10;
    }
    std::cout << "ans: " << ans <<"\n";

    int n = 0b10011110;
    int setbit;
    while(n) {
        setbit += (n>>1) & 1;
        // std::cout << "ans: " << setbit <<"\n";
        n = n >> 1;
    }
    // std::cout << "ans: " << setbit <<"\n";
    int msb = n>>4;
    std::cout << "msb: " << msb <<"\n";
    return 0;
}