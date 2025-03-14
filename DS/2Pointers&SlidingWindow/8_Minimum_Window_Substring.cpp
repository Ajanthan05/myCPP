#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>

using namespace std;

string minWindow(string s, string t) {
    int n = s.size();
    int m = t.size();
    
    if (n < m) return ""; // Edge case: if t is longer than s

    int minlen = INT_MAX;
    int startingInd = -1;

    for (int i = 0; i < n; i++) {
        int cnt = 0;
        int hash[256] = {0};

        // Populate the hash table with the frequency of characters in t
        for (int j = 0; j < m; j++) hash[t[j]]++;

        for (int j = i; j < n; j++) {
            if (hash[s[j]] > 0) cnt++;  
            hash[s[j]]--;

            // Check if all characters are matched
            if (cnt == m) {
                if (j - i + 1 < minlen) {
                    minlen = j - i + 1;
                    startingInd = i;
                }
                break; // No need to continue this loop
            }
        }
    }
    
    return (startingInd == -1) ? "" : s.substr(startingInd, minlen);
}

string minWindow(string s, string t) {
    int n = s.size();
    int m = t.size();
    
    if (n < m) return ""; // Edge case: if t is longer than s

    int minlen = INT_MAX;
    int startingInd = -1;
    int r=0, l=0;
    int hash[256] = {0};
    int cnt = 0;

    for(char c : t) hash[c]++;

    while(r < n) {
        if (hash[s[r]] > 0) cnt++;
        hash[s[r]]--;

        while (cnt == m) {
            if (r - l + 1 < minlen) {
                minlen = r - l + 1;
                startingInd = l;
            }
            hash[s[l]]++;
            if (hash[s[l]] > 0) cnt--;
            
            l++;
        }
        
        r++;
    }

    return (startingInd == -1) ? "" : s.substr(startingInd, minlen);
}

int main() {


    return 0;
}