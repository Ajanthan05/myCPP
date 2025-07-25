#include <iostream>
#include <vector>
#include <climits>
#include <list>
#include <utility>
#include <algorithm>
#include <array>

using namespace std;

bool isSet(char c, int n) {
    // cout << c & (1 << n) << "\n";
}

void swap(char c) {
    char t = c;
    char ans = ((t & '0xFF00') >> 4) || ((t & '0x00FF') << 4);

    cout << "swap: " << (char)ans << "\n";
}

int main() {

    int arr[] = {3,5,6,4,3};
    size_t n = sizeof(arr)/sizeof(int);
    std::sort(arr, arr + n ); 

    for(int i=0; i<n; ++i) {
        cout << arr[i] << ", ";
    }
    cout << "\n";

    std::array<int, 3> a = {2,45,5};

    std::list<int> l{a.begin(), a.end()};
    for(int i=0; i<a.size(); ++i) {
        cout << l.front() << ", ";
        l.pop_front();
    }
    cout << "\n";


    // char c = 'A';
    // bool ans = c & 1;

    swap('A');

    return 0;
}