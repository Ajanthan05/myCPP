#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>

using namespace std;

int f(int i, int j, vector<int> &arr) {
    if (i == j) return 0;

    int mini = INT_MAX;
    for(int k=i; k<j; k++) {
        int steps = arr[i-1] * arr[k] * arr[j] + f(i, k, arr)
                                               + f(k+1, j, arr);
        mini = min(mini, steps);
    }
    return mini;
}

int matrixMultiplication(vector<int> &arr, int N) {
    return f(1, N-1, arr);
}

/* MEMOIZATION */
int f(int i, int j, vector<int> &arr, vector<vector<int>> &dp) {
    if (i == j) return 0;

    if(dp[i][j] != -1) return dp[i][j];

    int mini = INT_MAX;
    for(int k=i; k<j; k++) {
        int steps = arr[i-1] * arr[k] * arr[j] + f(i, k, arr, dp)
                                               + f(k+1, j, arr, dp);
        mini = min(mini, steps);
    }
    return dp[i][j] = mini;
}

int matrixMultiplication(vector<int> &arr, int N) {
    vector<vector<int>> dp(N, vector<int>(N, -1));
    return f(1, N-1, arr, dp);
}

/*  TABULATION  
TC = O(N*N)*N = O(N^3)
SC = O(N^2) + O(N)
*/
int f(int i, int j, vector<int> &arr, vector<vector<int>> &dp) {
    if (i == j) return 0;

    if(dp[i][j] != -1) return dp[i][j];

    int mini = INT_MAX;
    for(int k=i; k<j; k++) {
        int steps = arr[i-1] * arr[k] * arr[j] + f(i, k, arr, dp)
                                               + f(k+1, j, arr, dp);
        mini = min(mini, steps);
    }
    return dp[i][j] = mini;
}

int matrixMultiplication(vector<int> &arr, int N) {
    vector<vector<int>> dp(N, vector<int>(N, 0));
    // for(int i=0; i<=N; i++) {
    //     for(int j=0; j<=N; j++) {
    //         if (i == j) dp[i][j] = 0;
    //     }
    // }
    // But this also no needed It's already zero
    for(int i=0; i<N; i++) {
        dp[i][i] = 0;
    }

    for(int i=N-1; i>=1; i--) {
        for(int j=i+1; j<N; j++) {

            int mini = INT_MAX;
            for(int k=i; k<j; k++) {
                int steps = arr[i-1] * arr[k] * arr[j] + dp[i][k]
                                                       + dp[k+1][j];
                mini = min(mini, steps);
            }
            dp[i][j] = mini;
        }
    }
    return dp[1][N-1];
}

int main() {


    return 0;
}