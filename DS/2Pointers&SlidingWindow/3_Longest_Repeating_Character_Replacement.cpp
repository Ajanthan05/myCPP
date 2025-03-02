#include <iostream>
#include <vector>
#include <string>

using namespace std;

// int characterReplacement(string s, int k) {
//     int n = s.length();
//     int maxlen = 0;

//     for(int i=0; i<n; i++) {
//         int hash[26] = {0};
//         int changes;
//         int maxfreq = 0;
//         for(int j=i; j<n; j++) {
//             hash[s[j] - 'A']++;
//             maxfreq = max(maxfreq, hash[j]);
//             changes = (j-i+1) - maxfreq;

//             if (changes <= k) {
//                 maxlen = max(maxlen, j-i+1);
//             }
//             else break;
//         }
//     }
//     return maxlen;
// }

int characterReplacement(string s, int k) {
    int n = s.length();
    int maxlen = 0;
    int hash[26] = {0};
    int maxfreq = 0;
    int l = 0;

    for(int r=0; r<n; r++) {
        hash[s[r] - 'A']++;
        maxfreq = max(maxfreq, hash[s[r] - 'A']);

        while((r-l + 1) - maxfreq > k) {
            hash[s[l] - 'A']--;
            l++;
        }
        maxlen = max(maxlen, (r - l + 1));
    }
    return maxlen;
}

int characterReplacement_Opt(string s, int k) {
    int n = s.length();
    int maxlen = 0;
    int hash[26] = {0};
    int maxfreq = 0;
    int l = 0;

    for(int r=0; r<n; r++) {
        hash[s[r] - 'A']++;
        maxfreq = max(maxfreq, hash[s[r] - 'A']);

        if((r-l + 1) - maxfreq > k) {
            hash[s[l] - 'A']--;
            l++;
        }
        maxlen = max(maxlen, (r - l + 1));
    }
    return maxlen;
}



struct Result {
    int maxLen;
    int start;
    int end;
};

Result characterReplacement_Opt_return_ind(const string &s, int k) {
    int n = s.length();
    int maxlen = 0;
    int bestL = 0;
    int bestR = 0;
    int hash[26] = {0};
    int maxfreq = 0;
    int l = 0;
    
    for (int r = 0; r < n; r++) {
        // Increase the count of the current character.
        hash[s[r] - 'A']++;
        // Update max frequency within the current window.
        maxfreq = max(maxfreq, hash[s[r] - 'A']);
        
        // If replacements required exceed k, shrink the window from the left.
        if ((r - l + 1) - maxfreq > k) {
            hash[s[l] - 'A']--;
            l++;
        }
        
        // Update the best window found so far.
        if ((r - l + 1) > maxlen) {
            maxlen = r - l + 1;
            bestL = l;
            bestR = r;
        }
    }
    
    return {maxlen, bestL, bestR};
}


int main() {

    string s = "AABABBA";  //"ABAB";
    cout << characterReplacement(s, 1) << "\n";

    string s2 = "AABABBAYYYYYYYYYZYYYYCYYY";  //"ABAB";
    cout << characterReplacement(s2, 1) << "\n";

    string s3 = "AABABBAYYYYYYYYYZYYYYCYYY";  //"ABAB";
    cout << characterReplacement_Opt(s3, 1) << "\n";

    string s4 = "AABABBA";
    int k = 1;
    Result res = characterReplacement_Opt_return_ind(s4, k);
    cout << "Max Length: " << res.maxLen << "\n"
         << "Start Index: " << res.start << "\n"
         << "End Index: " << res.end << endl;

    return 0;
}