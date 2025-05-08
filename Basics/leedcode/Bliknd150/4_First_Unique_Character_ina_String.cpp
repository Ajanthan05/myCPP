#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <queue>

using namespace std;

int firstUniqChar(string s) {
    vector<int> freq(26, 0);

    for(char c : s) {
        freq[c - 'a']++;
    } 
    
    for(int i=0; i<s.size(); i++) {
        if(freq[s[i] - 'a'] == 1) return i;
    }
    return -1;
}

int main() {


    return 0;
}