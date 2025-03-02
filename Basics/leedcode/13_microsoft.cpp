#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <numeric> // For std::iota

using namespace std;

int fun(vector<char> s, int k) {
    int n = s.size();
    vector<bool> police(n, 0);
    for(int it; it<n; it++) {
        if (s[it] == 'P') {
            std::iota(police[it], police[it+k], 1);
            continue;
        }
    }
}

int main() {

    vector<char> s = {'T', 'P', 'T', 'T'};
    return 0;
}