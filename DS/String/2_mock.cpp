#include <iostream>
#include <map>
#include <set>
#include <queue>
#include <string>
#include <algorithm>

using namespace std;

int permutation(string s1, string s2) {
    map<char, int> s2_freq, window_freq;
    int n = s1.size();
    int m = s2.size();
    if (m > n) return 0;

    for (int i=0; i<m; i++) {
        s2_freq[s2[i]]++;
    }

    for (int i=0; i<m; i++)  window_freq[s1[i]]++;

    int count = 0;
    if (window_freq == s2_freq) count++;

    for(int i=m; i<n; i++) {
        window_freq[s1[i]]++;
        window_freq[s1[i-m]]--;

        if (window_freq[s1[i-m]] == 0) window_freq.erase(s1[i-m]);

        if (window_freq == s2_freq) count++;
    }
    return count;
}

int main() {
    string s1 = "cbabcacabca";
    string s2 = "abc";

    cout << permutation(s1, s2) << "\n";

    return 0;
}