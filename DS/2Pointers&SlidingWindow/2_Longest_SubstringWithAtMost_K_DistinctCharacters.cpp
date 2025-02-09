#include <iostream>
#include <vector>
#include <climits>
// #include <map>
#include <unordered_map>

/*
TC = O(N) + O(N) + O(256)
SC = O(256)
*/
int longestSubstring(std::string s, int k) {
    int maxlen = 0;
    int n = s.size();
    int l = 0, r = 0;

    std::unordered_map<char, int> mpp;
    while(r<n) {
        mpp[s[r]]++;

        while(mpp.size() > k) {
            mpp[s[l]]--;
            if(mpp[s[l]] == 0) mpp.erase(s[l]); 
                
            l++;
        }

        if (mpp.size() <= k) maxlen = std::max(maxlen, r-l+1);

        r++;
    }
    return maxlen;
}

/*
TC = O(N) + O(N) + O(256)
SC = O(256)
*/
int longestSubstring_Opt(std::string s, int k) {
    int maxlen = 0;
    int n = s.size();
    int l = 0, r = 0;

    std::unordered_map<char, int> mpp;
    while(r<n) {
        mpp[s[r]]++;

        if(mpp.size() > k) {
            mpp[s[l]]--;
            if(mpp[s[l]] == 0) mpp.erase(s[l]); 
                
            l++;
        }

        if (mpp.size() <= k) maxlen = std::max(maxlen, r-l+1);

        r++;
    }
    return maxlen;
}

int main() {

    std::string s = "aaabbccd";
    std::cout << "longestSubstring: " << longestSubstring_Opt(s, 2) << "\n";
    return 0;
}