#include <iostream>
#include <vector>
#include <climits>

using namespace std;

int fun(int i, int j1, int j2, vector<vector<int>>& grid,int n, int m) {
    if (j1 < 0 || j1 >= m || j2 < 0 || j2 >= m) return INT_MIN;
    if (i == n-1) {
        if (j1 == j2) return grid[i][j1];
        else return grid[i][j1] + grid[i][j2];
    }
    int maxi = INT_MIN;
    for(int d1=-1; d1<=1; d1++) {
        for(int d2=-1; d2<=1; d2++) {
            int value = 0;
            if(j1 == j2) value = grid[i][j1];
            else value = grid[i][j1] + grid[i][j2];

            value += fun(i+1, j1+d1, j2+d2, grid, n, m);
            maxi = max(maxi, value);
        }
    }
    return maxi;
}


int cherryPickup(vector<vector<int>>& grid) {
    int n = grid.size();
    int m = grid[0].size();

    return fun(0, 0, m-1, grid, n, m);
}



int fun_dp(int i, int j1, int j2, vector<vector<int>>& grid,int n, int m, vector<vector<vector<int>>> &dp) {
    if (j1 < 0 || j1 >= m || j2 < 0 || j2 >= m) return INT_MIN;
    if (dp[i][j1][j2] != -1) return dp[i][j1][j2];
    if (i == n-1) {
        if (j1 == j2) return dp[i][j1][j2] = grid[i][j1];
        else return dp[i][j1][j2] = grid[i][j1] + grid[i][j2];
    }
    int maxi = INT_MIN;
    for(int d1=-1; d1<=1; d1++) {
        for(int d2=-1; d2<=1; d2++) {
            int value = 0;
            if(j1 == j2) value = grid[i][j1];
            else value = grid[i][j1] + grid[i][j2];

            value += fun_dp(i+1, j1+d1, j2+d2, grid, n, m, dp);
            maxi = max(maxi, value);
        }
    }
    return dp[i][j1][j2] =  maxi;
}


int cherryPickup_dp(vector<vector<int>>& grid) {
    int n = grid.size();  // r
    int m = grid[0].size();  // c

    vector<vector<vector<int>>> dp(n, vector<vector<int>>(m, vector<int>(m, -1)));

    return fun_dp(0, 0, m-1, grid, n, m, dp);
}

/*  TABULATION

*/

int cherryPickup_TABULATION(vector<vector<int>>& grid) {
    int n = grid.size();  // r
    int m = grid[0].size();  // c

    vector<vector<vector<int>>> dp(n, vector<vector<int>>(m, vector<int>(m, -1)));
    for(int j1=0; j1<m; j1++) {
        int maxc = INT_MIN;
        for(int j2=0; j2<m; j2++) {
            if (j1 == j2) dp[n-1][j1][j2] = grid[n-1][j1];
            else dp[n-1][j1][j2] = grid[n-1][j1] + grid[n-1][j2];
        }
    }

    for(int i=n-2; i>=0; i--) {
        for(int j1=0; j1<m; j1++) {
            for(int j2=0; j2<m; j2++) {
                int maxi = INT_MIN;
                for(int d1=-1; d1<=1; d1++) {
                    for(int d2=-1; d2<=1; d2++) {
                        int value = 0;
                        if(j1 == j2) value = grid[i][j1];
                        else value = grid[i][j1] + grid[i][j2];

                        if (j1+d1 >= 0 && j1+d1 < m && j2+d2 >= 0 && j2+d2 < m) {
                            value += dp[i+1][j1+d1][j2+d2]; 
                        }
                        else {
                            value += INT_MIN;
                        }
                        maxi = max(maxi, value);
                    }
                }
                dp[i][j1][j2] =  maxi;
            }
        }
    }

    return dp[0][0][m-1];
}

int cherryPickup_TABULATION_SpaceOptimize(vector<vector<int>>& grid) {
    int n = grid.size();  // r
    int m = grid[0].size();  // c

    // vector<vector<vector<int>>> dp(n, vector<vector<int>>(m, vector<int>(m, -1)));
    // int front[m][m];
    // int cur[m][m];
    vector<vector<int>> front(m, vector<int>(m, 0));
    vector<vector<int>> cur(m, vector<int>(m, 0));

    for(int j1=0; j1<m; j1++) {
        int maxc = INT_MIN;
        for(int j2=0; j2<m; j2++) {
            if (j1 == j2) front[j1][j2] = grid[n-1][j1];
            else front[j1][j2] = grid[n-1][j1] + grid[n-1][j2];
        }
    }

    for(int i=n-2; i>=0; i--) {
        for(int j1=0; j1<m; j1++) {
            for(int j2=0; j2<m; j2++) {
                int maxi = INT_MIN;
                for(int d1=-1; d1<=1; d1++) {
                    for(int d2=-1; d2<=1; d2++) {
                        int value = 0;
                        if(j1 == j2) value = grid[i][j1];
                        else value = grid[i][j1] + grid[i][j2];

                        if (j1+d1 >= 0 && j1+d1 < m && j2+d2 >= 0 && j2+d2 < m) {
                            value += front[j1+d1][j2+d2]; 
                        }
                        else {
                            value += INT_MIN;
                        }
                        maxi = max(maxi, value);
                    }
                }
                cur[j1][j2] =  maxi;
            }
        }
        front = cur;
    }

    return front[0][m-1];
}

int main() {

    vector<vector<int>> grid = {{3,1,1},{2,5,1},{1,5,5},{2,1,1}};
    cout << cherryPickup_dp(grid) << "\n";

    cout << cherryPickup_TABULATION(grid) << "\n";
    cout << cherryPickup_TABULATION_SpaceOptimize(grid) << "\n";

    return 0;
}