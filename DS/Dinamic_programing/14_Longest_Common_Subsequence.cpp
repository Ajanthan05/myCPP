#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>

using namespace std;

/*
TC = O(2^N * 2^M)
*/
int fun(int ind1, int ind2, string text1, string text2) {

    // if (ind1==0 || ind2==0) {
    //     if (text1[ind1] == text2[ind2]) return 1;
    //     else return 0;
    // }

    if (ind1 < 0 || ind2 < 0) {
        return 0;
    }

    // int match = INT_MIN;
    // int notMatch = INT_MIN;
    if (text1[ind1] == text2[ind2]) {
        return 1 + fun(ind1-1, ind2-1, text1, text2);
    }
    else {
        return 0 + max( fun(ind1 - 1, ind2, text1, text2), fun(ind1, ind2 - 1, text1, text2) );
    }

    // return max(match, notMatch);
}

int longestCommonSubsequence(string text1, string text2) {
    int ind1 = text1.size(), ind2 = text2.size();
    return fun(ind1 - 1, ind2 - 1, text1, text2);
}

/* 
Optimize the overlaping sub problem
TC = O(N*M)
SC = O(N*M) + O(N + M) // here O(N + M) is Auxiliary Stack Space 
*/
int fun_DP(int ind1, int ind2, string &text1, string &text2, vector<vector<int>> &dp) {

    if (ind1 < 0 || ind2 < 0) return 0;

    if (dp[ind1][ind2] != -1) return dp[ind1][ind2];

    if (text1[ind1] == text2[ind2]) {
        return dp[ind1][ind2] = 1 + fun_DP(ind1-1, ind2-1, text1, text2, dp);
    }
    else {
        return dp[ind1][ind2] = 0 + max( fun_DP(ind1 - 1, ind2, text1, text2, dp), fun_DP(ind1, ind2 - 1, text1, text2, dp) );
    }
}

int longestCommonSubsequence_o(string text1, string text2) {
    int ind1 = text1.size(), ind2 = text2.size();

    // vector<pair<int, int>> dp(min(ind1, ind2), pair<int, int>(-1, -1));
    vector<vector<int>> dp(ind1, vector<int>(ind2, -1));

    return fun_DP(ind1 - 1, ind2 - 1, text1, text2,dp);
}

/* TABULATION

*/
int longestCommonSubsequence_TABULATION(string text1, string text2) {
    int ind1 = text1.size(), ind2 = text2.size();
    vector<vector<int>> dp(ind1 + 1, vector<int>(ind2 + 1, 0));

    // Properly initialize base case (first row and first column should be 0)
    for(int i1 = 0; i1 <= ind1; i1++) dp[i1][0] = 0;
    for(int i2 = 0; i2 <= ind2; i2++) dp[0][i2] = 0;

    // Fill the DP table
    for(int i1 = 1; i1 <= ind1; i1++) {
        for(int i2 = 1; i2 <= ind2; i2++) {
            if (text1[i1 - 1] == text2[i2 - 1]) {
                dp[i1][i2] = 1 + dp[i1 - 1][i2 - 1];  // Match case
            } else {
                dp[i1][i2] = max(dp[i1 - 1][i2], dp[i1][i2 - 1]);  // No match case
            }
        }
    }

    // Debug Print (optional)
    for(int i1 = 0; i1 <= ind1; i1++) {
        for(int i2 = 0; i2 <= ind2; i2++) {
            cout << dp[i1][i2] << " ";
        }
        cout << "\n";
    }

    return dp[ind1][ind2];  // Final result is at dp[ind1][ind2]
}

// int longestCommonSubsequence_TABULATION(string text1, string text2) {
//     int ind1 = text1.size(), ind2 = text2.size();
//     vector<vector<int>> dp(ind1 + 1, vector<int>(ind2 + 1, 0));

//     for(int i1=0; i1<ind1; i1++) {
//         dp[i1][0] = 0;
//     }
//     for(int i2=1; i2<ind2; i2++) {
//         dp[0][i2] = 0;
//     }

//     for(int i1=1; i1<=ind1; i1++) {
//         for(int i2=1; i2<=ind2; i2++) {
//             if (text1[i1 - 1] == text2[i2 - 1]) {
//                 return dp[i1][i2] = 1 + dp[i1-1][i2-1];
//             }
//             else {
//                 return dp[i1][i2] = 0 + max( dp[i1 - 1][i2], dp[i1][i2 - 1] );
//             }
//         }
//     }
//     cout << "hi\n";
//     for(int i1=0; i1<=ind1; i1++) {
//         for(int i2=0; i2<=ind2; i2++) {
//             cout << dp[i1][i2] << " ";
//         }
//         cout << "\n";
//     }

//     return dp[ind1 - 1][ind2 - 1];
// }


/* SPACE OPTIMIZATION 

*/
// int longestCommonSubsequence_TABULATION_SO(string text1, string text2) {
//     int ind1 = text1.size(), ind2 = text2.size();
//     // vector<vector<int>> dp(ind1 + 1, vector<int>(ind2 + 1, 0));

//     vector<int> prev(ind2 + 1, 0), cur(ind2 + 1, 0);

//     // for(int i1=0; i1<ind1; i1++) {
//     //     prev[0] = 0;
//     // }
//     for(int i2=1; i2<ind2; i2++) { // Firest row eery one is zero
//         prev[i2] = 0;
//     }

//     for(int i1=1; i1<ind1; i1++) {
//         for(int i2=1; i2<ind1; i2++) {
//             if (text1[ind1 - 1] == text2[ind2 - 1]) {
//                 return cur[ind2] = 1 + prev[ind2-1];
//             }
//             else {
//                 return cur[ind2] = 0 + max( prev[ind2], cur[ind2 - 1] );
//             }
//         }
//         prev = cur;
//     }
//     return prev[ind2 - 1];
// }
int longestCommonSubsequence_TABULATION_SO(string text1, string text2) {
    int ind1 = text1.size(), ind2 = text2.size();
    
    vector<int> prev(ind2 + 1, 0), cur(ind2 + 1, 0);

    for (int i1 = 1; i1 <= ind1; i1++) {
        for (int i2 = 1; i2 <= ind2; i2++) {
            if (text1[i1 - 1] == text2[i2 - 1]) {
                cur[i2] = 1 + prev[i2 - 1];  // Matching case
            } else {
                cur[i2] = max(prev[i2], cur[i2 - 1]);  // No match case
            }
        }
        prev = cur;  // Update prev for the next iteration
    }
    return prev[ind2];
}






/////////////////////////
int findLCS_longestCommonSubsequence_TABULATION(string text1, string text2, vector<vector<int>> &dp) {
    int ind1 = text1.size(), ind2 = text2.size();

    // Properly initialize base case (first row and first column should be 0)
    for(int i1 = 0; i1 <= ind1; i1++) dp[i1][0] = 0;
    for(int i2 = 0; i2 <= ind2; i2++) dp[0][i2] = 0;

    // Fill the DP table
    for(int i1 = 1; i1 <= ind1; i1++) {
        for(int i2 = 1; i2 <= ind2; i2++) {
            if (text1[i1 - 1] == text2[i2 - 1]) {
                dp[i1][i2] = 1 + dp[i1 - 1][i2 - 1];  // Match case
            } else {
                dp[i1][i2] = max(dp[i1 - 1][i2], dp[i1][i2 - 1]);  // No match case
            }
        }
    }

    // Debug Print (optional)
    for(int i1 = 0; i1 <= ind1; i1++) {
        for(int i2 = 0; i2 <= ind2; i2++) {
            cout << dp[i1][i2] << " ";
        }
        cout << "\n";
    }

    return dp[ind1][ind2];  // Final result is at dp[ind1][ind2]
}

// TC = O(N+M) when worst case back tracking
string findLCS(int ind1, int ind2,string &s1, string &s2){
    vector<vector<int>> dp(ind1 + 1, vector<int>(ind2 + 1, 0));
	findLCS_longestCommonSubsequence_TABULATION(s1, s2, dp);

    string s = "";
    while (ind1 > 0 && ind2 > 0) {    
        if (s1[ind1-1] == s2[ind2-1]) {

            s = s1[ind1-1] + s;
            // Mode back diognaly
            ind1--;
            ind2--;
        }
        else {
            if (dp[ind1-1][ind2] >= dp[ind1][ind2-1]) {
                ind1--;
            }
            else {
                ind2--;
            }
        }
    }
    return s;
}


/*      Longest Palindromic Subsequence     */
int longestPalindromeSubseq(string s) {
    string r = s;
    reverse(r.begin(), r.end());
    return longestCommonSubsequence_TABULATION_SO(s, r);
}

/*  Minimum Insertions to Make String Palindrome */
int minInsertions(string s) {
    return s.size() - longestPalindromeSubseq(s);
}

/*  Minimum Insertions/Deletions to Convert String A to String B    */
int canYouMake(string &str, string &ptr)
{
    return str.length() + ptr.length() - 2*longestCommonSubsequence_TABULATION_SO(str, ptr);
}

/*  Shortest Common Supersequence   */
string shortestCommonSupersequence(string str1, string str2) {
        
}

int main() {
    string text1 = "abcde";
    string text2 = "ace";
    cout << longestCommonSubsequence_TABULATION(text1, text2) << "\n";
    cout << findLCS(5, 3, text1, text2) << "\n";

    string text3 = "abcde";
    string text4 = "bdqek";
    cout << findLCS(5, 5, text3, text4) << "\n";

    return 0;
}