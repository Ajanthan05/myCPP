#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>

using namespace std;

int fun(int i, int j, string s1, string s2, vector<vector<int>> &dp) {

    // Base case
    // s1 Get end
    if (i < 0) return j + 1;
    else if (j < 0) return i + 1;

    if (dp[i][j] != -1) return dp[i][j];

    if (s1[i] == s2[j]) return 0 + fun(i-1, j-1, s1, s2, dp);
    else {
        return dp[i][j] = min(  1 + fun(i, j-1, s1, s2, dp), // Insert
                          min(  1 + fun(i-1, j, s1, s2, dp), // Delete
                                1 + fun(i-1, j-1, s1, s2, dp) // Replace
                                ));
    }
}

int minDistance(string word1, string word2) {
    int n = word1.size(), m = word2.size();
    vector<vector<int>> dp(n, vector<int>(m, -1));
    return fun(n-1, m-1, word1, word2, dp);
}

/* TABULATION */
// int fun_TABULATION(int i, int j, string s1, string s2, vector<vector<int>> &dp) {

//     if (i == 0) return j;
//     else if (j == 0) return i;

//     if (dp[i][j] == -1) return dp[i][j];

//     if (s1[i-1] == s2[j-1]) return 0 + fun_TABULATION(i-1, j-1, s1, s2, dp);
//     else {
//         return dp[i][j] = min(  1 + fun_TABULATION(i, j-1, s1, s2, dp), // Insert
//                           min(  1 + fun_TABULATION(i-1, j, s1, s2, dp), // Delete
//                                 1 + fun_TABULATION(i-1, j-1, s1, s2, dp) // Replace
//                                 ));
//     }
// }

// int minDistance(string word1, string word2) {
//     int n = word1.size(), m = word2.size();
//     vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
//     return fun_TABULATION(n, m, word1, word2, dp);
// }

int minDistance(string s1, string s2) {
    int n = s1.size(), m = s2.size();
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));

    for(int i=0; i<=n; i++) dp[i][0] = i;
    for(int j=0; j<=m; j++) dp[0][j] = j;

    for(int i=1; i<=n; i++) {
        for(int j=1; j<=m; j++) {
            if (s1[i-1] == s2[j-1])  dp[i][j] = 0 + dp[i-1][j-1];
            else {
                dp[i][j] = 1 +  min(dp[i][j-1], // Insert
                                min(dp[i-1][j], // Delete
                                    dp[i-1][j-1] // Replace
                                        ));
            }
        }
    }
    return dp[n][m];
}

/* SPACE OPTIMIZE */
int minDistance(string s1, string s2) {
    int n = s1.size(), m = s2.size();

    vector<int> prev(m+1, 0), cur(m+1, 0);

    // for(int i=0; i<=n; i++) dp[i][0] = i;
    for(int j=0; j<=m; j++) prev[j] = j;

    for(int i=1; i<=n; i++) {
        cur[0] = i; // for(int i=0; i<=n; i++) dp[i][0] = i;
        for(int j=1; j<=m; j++) {
            if (s1[i-1] == s2[j-1])  cur[j] = 0 + prev[j-1];
            else {
                cur[j] = 1 +  min(cur[j-1], // Insert
                                min(prev[j], // Delete
                                    prev[j-1] // Replace
                                        ));
            }
        }
        prev = cur;
    }
    return cur[m];
}

int main() {


    return 0;
}