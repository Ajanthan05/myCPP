#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <limits>

using namespace std;

// int recursion(int i, int j, vector<vector<int>>& matrix, int n, int m) {
//     if (j < 0 || j >= m) return INT_MAX;
//     if (i == 0) return matrix[0][j];

//     int c =  matrix[i][j] + recursion(i-1, j, matrix, n, m);
//     int ld = matrix[i][j] + recursion(i-1, j-1, matrix, n, m);
//     int rd = matrix[i][j] + recursion(i-1, j+1, matrix, n, m);

//     return min(c, min(ld, rd));
// }

// int minFallingPathSum(vector<vector<int>>& matrix) {
//     int n = matrix.size();
//     int m = matrix[0].size();
//     int minSum = INT_MAX;

//     for(int j=0; j<m; j++) {
//         minSum = min(minSum, recursion(n-1, j, matrix, n, m));
//     }
     

//     return minSum;
// }

long long recursion(int i, int j, const vector<vector<int>>& matrix, int n, int m) {
    if (j < 0 || j >= m) return numeric_limits<long long>::max(); // Out-of-bound column.
    if (i == 0) return matrix[0][j];       // Base case: first row.

    long long up   = recursion(i - 1, j, matrix, n, m);
    long long left = recursion(i - 1, j - 1, matrix, n, m);
    long long right= recursion(i - 1, j + 1, matrix, n, m);

    long long minAbove = min(up, min(left, right));
    return matrix[i][j] + minAbove;
}

int minFallingPathSum(vector<vector<int>>& matrix) {
    int n = matrix.size();
    int m = matrix[0].size();
    long long minSum = numeric_limits<long long>::max();

    // Try each column of the bottom row.
    for (int j = 0; j < m; j++) {
        minSum = min(minSum, recursion(n - 1, j, matrix, n, m));
    }
    
    return static_cast<int>(minSum);
}



int add(int a, int b) {
    // If a is INT_MAX, we treat the sum as INT_MAX to denote an invalid/overflow path.
    if(a == INT_MAX) return INT_MAX;
    return a + b;
}

int recursion_add(int i, int j, const vector<vector<int>>& matrix, int n, int m) {
    if (j < 0 || j >= m) return INT_MAX; // Out-of-bound column.
    if (i == 0) return matrix[0][j];       // Base case: first row.

    int up   = recursion(i - 1, j, matrix, n, m);
    int left = recursion(i - 1, j - 1, matrix, n, m);
    int right= recursion(i - 1, j + 1, matrix, n, m);

    int sumUp   = add(up, matrix[i][j]);
    int sumLeft = add(left, matrix[i][j]);
    int sumRight= add(right, matrix[i][j]);

    return min(sumUp, min(sumLeft, sumRight));
}

int minFallingPathSum_with_add(vector<vector<int>>& matrix) {
    int n = matrix.size();
    int m = matrix[0].size();
    int minSum = INT_MAX;

    // Try each column of the bottom row.
    for (int j = 0; j < m; j++) {
        minSum = min(minSum, recursion_add(n - 1, j, matrix, n, m));
    }
    
    return minSum;
}



int main() {

    vector<vector<int>> matrix = {{2,1,3},{6,5,4},{7,8,9}};
    cout << minFallingPathSum(matrix) << "\n";

    return 0;
}