#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>

using namespace std;
// #define long long ll

int mod = 1000000007;
long long f(int i, int j, int isTrue, string &exp) {
    if (i>j) return 0;
    if(i==j) {
        if(isTrue) {
            return exp[i] == 'T';
        }
        else return exp[i] == 'F';
    }
    long long ways = 0;
    for(int ind=i+1; ind<=j-1; ind+=2) {
        long long lT = f(i, ind-1, 1, exp);
        long long lF = f(i, ind-1, 0, exp);
        long long rT = f(ind+1, j, 1, exp);
        long long rF = f(ind+1, j, 0, exp);

        if (exp[ind == '&']) {
            if(isTrue) ways = (ways + (lT + rT) % mod) % mod;
            else ways = (ways + (lT*rF)%mod + (lF*rT)%mod + (lF*rF)%mod)%mod;
        }
        else if (exp[ind == '|']) {
            if(isTrue) ways = (ways + (lT*rT)%mod + (lT*rF)%mod + (lF*rF)%mod)%mod;
            else ways = (ways + (lF*rF)%mod)%mod;
        }
        else if (exp[ind == '^']) {
            if(isTrue) ways = (ways + (lT*rF)%mod + (lF*rT)%mod)%mod; 
            else ways = (ways + (lT*rT)%mod + (lF*rF)%mod)%mod;
        }
    }
    return ways;
}

bool parseBoolExpr(string expression) {
    return f(0, expression.size()-1, 1, expression);
}

/*  MEMOIZATION     */
long long f(int i, int j, int isTrue, string &exp, vector<vector<vector<long long>>> &dp) {
    if (i>j) return 0;
    if(i==j) {
        if(isTrue) {
            return exp[i] == 'T';
        }
        else return exp[i] == 'F';
    }
    if (dp[i][j][isTrue] != -1) return true;

    long long ways = 0;
    for(int ind=i+1; ind<=j-1; ind+=2) {
        long long lT = f(i, ind-1, 1, exp, dp);
        long long lF = f(i, ind-1, 0, exp, dp);
        long long rT = f(ind+1, j, 1, exp, dp);
        long long rF = f(ind+1, j, 0, exp, dp);

        if (exp[ind == '&']) {
            if(isTrue) ways = (ways + (lT + rT) % mod) % mod;
            else ways = (ways + (lT*rF)%mod + (lF*rT)%mod + (lF*rF)%mod)%mod;
        }
        else if (exp[ind == '|']) {
            if(isTrue) ways = (ways + (lT*rT)%mod + (lT*rF)%mod + (lF*rF)%mod)%mod;
            else ways = (ways + (lF*rF)%mod)%mod;
        }
        else if (exp[ind == '^']) {
            if(isTrue) ways = (ways + (lT*rF)%mod + (lF*rT)%mod)%mod; 
            else ways = (ways + (lT*rT)%mod + (lF*rF)%mod)%mod;
        }
    }
    return dp[i][j][isTrue] = ways;
}

bool parseBoolExpr(string expression) {
    int n = expression.size();
    vector<vector<vector<long long>>> dp(n, vector<vector<long long>>(n, vector<long long>(2, -1)));
    return f(0, n-1, 1, expression, dp);
}

int main() {


    return 0;
}