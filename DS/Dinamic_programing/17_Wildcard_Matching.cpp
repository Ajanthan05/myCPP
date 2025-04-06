#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <utility>
#include <algorithm>

using namespace std;

// bool fun(int i, int j, string s, string p) {

//     // Base Case
//     // s get exhausted
//     if (i < 0) {
//         // p also get excosted
//         if (j < 0) return true;
//         else return false;
//     }
//     if (j < 0 && i >= 0) {
//         for(int ind=0; ind<=i; ind++) {
//             if (s[ind] != '*') return false;
//         }
//         return true;
//     }



//     if (s[i] == p[j] || s[i] == '?') {
//         return fun(i-1, j-1, s, p);
//     }
//     else if(s[i] == '*') {
//         // * can be nothing || mach the string from p
//         return fun(i-1, j, s, p) || fun(i, j-1, s, p);
//     }

//     return false;
// }

// bool isMatch(string s, string p) {
//     int n = s.size(), m = p.size();
//     return fun(n, m, s, p);
// }

bool fun(int i, int j, string s, string p) {
    // Base Case: s is exhausted
    if (i < 0) {
        // Check if remaining pattern consists only of '*'
        for(int ind = 0; ind <= j; ind++) {
            if (p[ind] != '*') return false;
        }
        return true;
    }

    // Base Case: p is exhausted but s is not
    if (j < 0) return false;

    // Match case
    if (s[i] == p[j] || p[j] == '?') {
        return fun(i - 1, j - 1, s, p);
    }
    // Wildcard '*' case
    else if (p[j] == '*') {
        // '*' can match nothing (move j) OR match a character in s (move i)
        return fun(i, j - 1, s, p) || fun(i - 1, j, s, p);
    }

    return false;
}

bool isMatch(string s, string p) {
    int n = s.size(), m = p.size();
    return fun(n - 1, m - 1, s, p);  // Fix the function call
}


/* */
bool fun(int i, int j, string s, string p, vector<vector<int>> &dp) {
    if (i < 0) {
        for(int ind = 0; ind <= j; ind++) {
            if (p[ind] != '*') return false;
        }
        return true;
    }

    if (j < 0) return false;

    if (dp[i][j] != -1) return dp[i][j];

    if (s[i] == p[j] || p[j] == '?') {
        return dp[i][j] = fun(i - 1, j - 1, s, p);
    }
    else if (p[j] == '*') {
        return dp[i][j] = fun(i, j - 1, s, p) || fun(i - 1, j, s, p);
    }

    return false;
}

bool isMatch(string s, string p) {
    int n = s.size(), m = p.size();
    vector<vector<int>> dp(n, vector<int>(m, -1));
    return fun(n - 1, m - 1, s, p, dp);  // Fix the function call
}


int main() {


    return 0;
}