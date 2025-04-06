#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>

using namespace std;

bool isPalindrome(int start, string s, int end) {
    while(start<=end) {
        if(s[start++] != s[end--]) {
            return false;
        }
    }
    return true;
}

vector<vector<string>> f(int ind, string s, vector<vector<string>> &ans, 
    vector<string> &path, int n) {

    if (ind == n) {
        ans.push_back(path);
        return ans;
    } 
    for(int i=ind; i<n; i++) {
        if(isPalindrome(ind, s, i)) {
            path.push_back(s.substr(ind, i - ind + 1));
            f(i+1, s, ans, path, n);
            path.pop_back();
        }
    }
    return ans;
}

vector<vector<string>> partition(string s) {
    int n = s.size();
    vector<vector<string>> ans;
    vector<string> path;

    f(0, s, ans, path, n);
    return ans;
}

/* DP */
class Solution {
public:
    vector<vector<string>> ans;
    vector<string> path;
    vector<vector<bool>> dp;

    void precomputePalindromes(string &s, int n) {
        dp = vector<vector<bool>>(n, vector<bool>(n, false));

        for (int i = 0; i < n; i++) {
            dp[i][i] = true; // Single-character substrings are always palindromes.
        }

        for (int len = 2; len <= n; len++) { // Checking all substring lengths
            for (int i = 0; i <= n - len; i++) {
                int j = i + len - 1;
                if (s[i] == s[j]) {
                    dp[i][j] = (len == 2) ? true : dp[i + 1][j - 1];
                }
            }
        }
    }

    void f(int ind, string &s, int n) {
        if (ind == n) {
            ans.push_back(path);
            return;
        }

        for (int i = ind; i < n; i++) {
            if (dp[ind][i]) {  // Use precomputed DP table instead of isPalindrome()
                path.push_back(s.substr(ind, i - ind + 1));
                f(i + 1, s, n);
                path.pop_back();
            }
        }
    }

    vector<vector<string>> partition(string s) {
        int n = s.size();
        precomputePalindromes(s, n);
        f(0, s, n);
        return ans;
    }
};
    
/* OPIMAL*/
class Solution {
public:
    vector<vector<string>> partition(string s) {
        vector<vector<string>> ans;
        vector<string> curr;
        recur(s,0,"",curr,ans);
        return ans;
        // return{{"ABc"}};
    }
    void recur(string& s, int ind, string str, vector<string>& curr,
                vector<vector<string>>& ans) {
        if (ind == s.length()) {
            ans.push_back(curr);
        }
        
        for (int i = ind; i < s.length(); i < i++){
            str+=s[i];
            if (palin(str)){
                curr.push_back(str);
                recur(s,i+1,"",curr,ans);
                curr.pop_back();
            }
        }
    }

    bool palin( string& s) {
        int left=0,right=s.length()-1;
        // Check if the substring s[left:right+1] is a palindrome
        while (left < right) {
            if (s[left++] != s[right--]) {
                return false;
            }
        }
        return true;
    }
    // bool palin(string &s){
    //     string str=s;
    //     reverse(str.begin(),str.end());
    //     if (str==s){
    //         return true;
    //     }
    //     return false;
    // }
};
// auto init = atexit([]() { ofstream("display_runtime.txt") << "0";});

int main() {


    return 0;
}