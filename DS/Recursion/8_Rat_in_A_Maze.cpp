#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>

using namespace std;

// void solve(int i, int j, vector<vector<int>> &arr, int n, 
//     vector<string> &ans, string move, vector<vector<int>> &vis) {

//     if (i == n && j == n-1) {
//         ans.push_back(move);
//         return;
//     }

//     // down
//     if (i+1<n && !vis[i+1][j] && arr[i+1][j] == 1) {
//         vis[i][j] = 1;
//         solve(i+1, j, arr, n, ans, move + 'D', vis);
//         vis[i][j] = 0;
//     }

//     // left
//     if (j-1 >= 0 && !vis[i][j-1] && arr[i][j-1] == 1) {
//         vis[i][j] = 1;
//         solve(i, j-1, arr, n, ans, move + 'L', vis);
//         vis[i][j] = 0;
//     }

//     // right
//     if (j+1<n && !vis[i][j+1] && arr[i][j+1] == 1) {
//         vis[i][j] = 1;
//         solve(i, j+1, arr, n, ans, move + 'R', vis);
//         vis[i][j] = 0;
//     }

//     // up
//     if (i-1 >= 0 && !vis[i-1][j] && arr[i-1][j] == 1) {
//         vis[i][j] = 1;
//         solve(i-1, j, arr, n, ans, move + 'U', vis);
//         vis[i][j] = 0;
//     }

// }

// vector<string> searchMaze(vector<vector<int>> &arr, int n) {
//     vector<string> ans;
//     vector<vector<int>> vis(n, vector<int>(n, 0));
//     if(arr[0][0] == 1) solve(0,0, arr, n, ans, "", vis);
//     return ans;
// }

/* Cleanup */
void solve(int i, int j, vector<vector<int>> &arr, int n, 
    vector<string> &ans, string move, vector<vector<int>> &vis, vector<int> di, vector<int> dj) {

    if (i == n-1 && j == n-1) {
        ans.push_back(move);
        return;
    }

    string dir = "DLRU";
    for(int ind=0; ind<4; ind++) {
        int nexti = i + di[ind];
        int nextj = j + dj[ind];
        if (nexti >= 0 && nextj >= 0 && nexti < n && nextj < n && !vis[nexti][nextj] && arr[nexti][nextj] == 1) {
            vis[i][j] = 1;
            solve(nexti, nextj, arr, n, ans, move + dir[ind], vis, di, dj);
            vis[i][j] = 0;
        }
    }
}

vector<string> searchMaze(vector<vector<int>> &arr, int n) {
    vector<string> ans;
    vector<vector<int>> vis(n, vector<int>(n, 0));
    vector<int> di = {1, 0, 0, -1};
    vector<int> dj = {0, -1, 1, 0};
    if(arr[0][0] == 1) solve(0,0, arr, n, ans, "", vis, di, dj);
    return ans;
}

/*
You are given an m x n integer array grid where grid[i][j] could be:

1 representing the starting square. There is exactly one starting square.
2 representing the ending square. There is exactly one ending square.
0 representing empty squares we can walk over.
-1 representing obstacles that we cannot walk over.
*/

int dfs(vector<vector<int>>& grid, int i, int j, int zero, int n, int m) {

    if (i < 0 || i >= n || j < 0 || j >= m || grid[i][j] == -1) return 0;

    if (grid[i][j] == 2) {
        return (zero == -1) ? 1 : 0;
    }

    grid[i][j] = -1;
    zero--;
    int totalPaths = dfs(grid, i+1, j, zero, n, m) + 
                     dfs(grid, i-1, j, zero, n, m) + 
                     dfs(grid, i, j+1, zero, n, m) + 
                     dfs(grid, i, j-1, zero, n, m);
    
    grid[i][j] = 0; // Backtrack
    zero++;
    return totalPaths;
}

int uniquePathsIII(vector<vector<int>>& grid) {
    int n = grid.size();
    int m = grid[0].size();
    int si, sj, zero = 0;

    for(int i=0; i<n; i++) {
        for(int j=0; j<m; j++) {
            if (grid[i][j] == 0) zero++;
            else if (grid[i][j] == 1) {
                si = i;
                sj = j;
            }
        }
    }
    return dfs(grid, si, sj, zero, n, m);
}
int main() {
    /*
    1 0 0 0
    1 1 0 0
    1 1 0 0
    0 1 1 1 
    Expected Output:
    DDRDRR DRDDRR 
*/

    vector<vector<int>> arr = {{1,0,0,0},{1,1,0,0}, {1,1,0,0},{0,1,1,1}};
    vector<string>  out = searchMaze(arr, 4);
    for(auto it : out) {
        cout << it << " ";
    }
    cout << "\n";
    return 0;
}