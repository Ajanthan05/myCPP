#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>

using namespace std;

int longestCommonSubstr(string& s1, string& s2) {
    int ind1 = s1.size(), ind2 = s2.size();
    vector<vector<int>> dp(ind1 + 1, vector<int>(ind2 + 1, 0));

    int ans = 0;
    for(int i1 = 1; i1 <= ind1; i1++) {
        for(int i2 = 1; i2 <= ind2; i2++) {
            if (s1[i1 - 1] == s2[i2 - 1]) {
                dp[i1][i2] = 1 + dp[i1 - 1][i2 - 1];  // Match case
                ans = max(ans, dp[i1][i2]);
            } 
        }
    }
    return ans;
}

/*  Longest Palindromic Subsequence */

int main() {


    return 0;
}