#include <iostream>
#include <vector>
#include <climits>

int mod = (int)(1e9 + 7);

/*  TC = O( 2^(M*N) )
    SC = O(PATH LENGTH) = O((M-1) + (N-1))
*/
int f(int i, int j, std::vector<std::vector<int>> &mat) {
    if (i >= 0 && j >= 0 && mat[i][j]== -1) return 0;
    if (i==0 && j==0) return 1;
    if (i<0 || j<0) return 0;

    int up = f(i-1, j, mat);
    int left = f(i, j-1, mat);

    return (up + left) % mod;
}

/*  DP
    TC = O(M*N)
    SC = O(PATH LENGTH) + For DP = O((M-1) + (N-1)) + O(N*M)
*/

int f_DP(int i, int j, std::vector<std::vector<int>> &dp, std::vector<std::vector<int>> &mat) {
    if (i>=0 && j>=0 && mat[i][j] == -1) return 0;
    if (i==0 && j==0) return 1;
    if (i<0 || j<0) return 0;

    if (dp[i][j] != -1) return dp[i][j];

    int up = f_DP(i-1, j, dp, mat);
    int left = f_DP(i, j-1, dp, mat);

    return dp[i][j] = (up + left) % mod;
}

/*  Tabulation
    TC = O(M*N)
    SC = O(N*M)  // for dp
*/
int f_T(int n, int m) {
    std::vector<std::vector<int>> dp(m, std::vector<int>(n, 0));
    dp[0][0] = 1;
    for (int i=0; i<n; i++) {
        for (int j=0; j<m; j++) {
            if (i==0 && j==0) continue;  // or dp[0][0] = 1
            else {
                int up = 0;
                if (i>0) up = dp[i-1][j];
                int left = 0;
                if (j>0) left = dp[i][j-1];
                dp[i][j] = up + left;
            }
         }
    }

    return dp[n-1][m-1];
}

/*  Tabulation
    TC = O(M*N)
    SC = O(N*M)  // for dp
*/
int f_T_SO(int n, int m) {
    std::vector<int> prev(n, 0);
    for (int i=0; i<n; i++) {
        std::vector<int> cur(n, 0);
        for (int j=0; j<m; j++) {
            if (i==0 && j==0) cur[j] = 1;  //continue;  // or dp[0][0] = 1
            else {
                int up = 0;
                if (i>0) up = prev[j];
                int left = 0;
                if (j>0) left = cur[j-1];
                cur[j] = up + left;
            }
         }
         prev = cur;
    }

    return prev[m-1];
}

int main() {
    int m = 3, n = 3;
    std::vector<std::vector<int>> mat = {{0,0,0}, {0,-1,0}, {0,0,0}};
    std::cout << f(m-1, n-1, mat) << "\n";

    int m2 = 3, n2 = 3;
    std::vector<std::vector<int>> dp2(m2, std::vector<int>(n2, -1));
    std::vector<std::vector<int>> mat2 = {{0,0,0}, {0,-1,0}, {0,0,0}};
    std::cout << f_DP(m2-1, n2-1, dp2, mat2) << "\n";

    int m3 = 3, n3 = 3;
    std::cout << f_T(m3, n3) << "\n";

    int m4 = 3, n4 = 3;
    std::cout << f_T_SO(n4, m4) << "\n";


    return 0;
}