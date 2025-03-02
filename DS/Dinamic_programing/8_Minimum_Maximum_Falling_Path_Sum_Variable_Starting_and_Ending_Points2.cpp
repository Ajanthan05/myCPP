#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <limits>

using namespace std;


int add(int a, int b) {
    // If a is INT_MAX, we treat the sum as INT_MAX to denote an invalid/overflow path.
    if(a == INT_MAX) return INT_MAX;
    return a + b;
}

/* MEMOIZATION
TC = O(N*M)
SC = O(N*M) + O(N)
*/
int recursion(int i, int j, const vector<vector<int>>& matrix, int n, int m, vector<vector<int>> &dp) {
    if (j < 0 || j >= m) return INT_MAX; // Out-of-bound column.
    if (i == 0) return matrix[0][j];       // Base case: first row.
    if (dp[i][j] != -1) return dp[i][j];
    int up   = recursion(i - 1, j, matrix, n, m, dp);
    int left = recursion(i - 1, j - 1, matrix, n, m, dp);
    int right= recursion(i - 1, j + 1, matrix, n, m, dp);

    int sumUp   = add(up, matrix[i][j]);
    int sumLeft = add(left, matrix[i][j]);
    int sumRight= add(right, matrix[i][j]);

    return dp[i][j] = min(sumUp, min(sumLeft, sumRight));
}

int minFallingPathSum(vector<vector<int>>& matrix) {
    int n = matrix.size();
    int m = matrix[0].size();
    int minSum = INT_MAX;

    // Try each column of the bottom row.
    for (int j = 0; j < m; j++) {
        vector<vector<int>> dp(m, vector<int>(n, -1));
        minSum = min(minSum, recursion(n - 1, j, matrix, n, m, dp));
    }
    
    return minSum;
}

/* TABULATION
TC = O(N*M)
SC = O(N*M) + O(N)
*/
// int recursion(int i, int j, const vector<vector<int>>& matrix, int n, int m, vector<vector<int>> &dp) {
//     if (j < 0 || j >= m) return INT_MAX; // Out-of-bound column.
//     if (i == 0) return matrix[0][j];       // Base case: first row.
//     if (dp[i][j] != -1) return dp[i][j];
//     int up   = recursion(i - 1, j, matrix, n, m, dp);
//     int left = recursion(i - 1, j - 1, matrix, n, m, dp);
//     int right= recursion(i - 1, j + 1, matrix, n, m, dp);

//     int sumUp   = add(up, matrix[i][j]);
//     int sumLeft = add(left, matrix[i][j]);
//     int sumRight= add(right, matrix[i][j]);

//     return dp[i][j] = min(sumUp, min(sumLeft, sumRight));
// }

// int minFallingPathSum(vector<vector<int>>& matrix) {
//     int n = matrix.size();
//     int m = matrix[0].size();
//     int minSum = INT_MAX;

//     // Try each column of the bottom row.
//     for (int j = 0; j < m; j++) {
//         vector<vector<int>> dp(m, vector<int>(n, -1));
//         minSum = min(minSum, recursion(n - 1, j, matrix, n, m, dp));
//     }
    
//     return minSum;
// }


int minFallingPathSum_Tabulation(vector<vector<int>>& matrix) {
    int n = matrix.size();
    int m = matrix[0].size();
    int minSum = 1e8;

    int dp[n][m] = {0};

    for(int j=0; j<m; j++) {
        dp[0][j] = matrix[0][j];
    }

    for(int i=1; i<n; i++) {
        for(int j=0; j<m; j++) {
            int up = matrix[i][j] + dp[i-1][j];
            int ld = 1e8, rd = 1e8;
            if ((j-1) >= 0) ld = matrix[i][j] + dp[i-1][j-1];
            if ((j+1) < m)  rd = matrix[i][j] + dp[i-1][j+1];

            dp[i][j] = min(up, min(ld, rd));
        }
    }

    int minc = 1e8;
    for(int j=0; j<m; j++) {
        minc = min(minc, dp[n-1][j]);
    }
    return minc;
}

int main() {

    vector<vector<int>> matrix = {{2,1,3},{6,5,4},{7,8,9}};
    // cout << minFallingPathSum(matrix) << "\n";

    cout << "Tabulation: " << minFallingPathSum_Tabulation(matrix) << "\n";

    return 0;
}