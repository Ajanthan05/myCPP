#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>

using namespace std;
/*
TC O(N) * O(K)
SC = O(N) + O(N)//ASS
*/
int f(int ind, vector<int>& arr, int k, int n, vector<int> &dp) {

    if (ind == n) return 0;

    if (dp[ind] != -1) return dp[ind];

    int len = 0;
    int maxi = INT_MIN;
    int maxAns = INT_MIN;

    for(int j=ind; j<min(n, ind + k); j++) {
        len++;
        maxi = max(maxi, arr[j]);
        int sum = len * maxi + f(j+1, arr, k, n, dp);
        maxAns = max(maxAns, sum);
    }
    return dp[ind] = maxAns;
}

int maxSumAfterPartitioning(vector<int>& arr, int k) {
    int n = arr.size();
    vector<int> dp(n, -1);
    return f(0, arr, k, n, dp);   
}

/* TABULATION */
int maxSumAfterPartitioning(vector<int>& arr, int k) {
    int n = arr.size();
    vector<int> dp(n+1, 0);

    for(int ind=n-1; ind>=0; ind--) {
        int len = 0;
        int maxi = INT_MIN;
        int maxAns = INT_MIN;

        for(int j=ind; j<min(n, ind + k); j++) {
            len++;
            maxi = max(maxi, arr[j]);
            int sum = len * maxi + dp[j+1];
            maxAns = max(maxAns, sum);
        }
        dp[ind] = maxAns;
    }

    return dp[0];   
}

int main() {


    return 0;
}