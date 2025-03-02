#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <utility>
#include <algorithm>

using namespace std;

/* Carry base cordinates */
void dfs(vector<vector<int>> &grid, int row, int col, vector<vector<int>> &vis, 
    vector<pair<int, int>> &island, int rowBase, int colBase) {
        int n = grid.size();
        int m = grid[0].size();
        vis[row][col] = 1;
        island.push_back({row - rowBase, col - colBase});



        int delrow[] = {-1, 0, 1,  0};
        int delcol[] = { 0, 1, 0, -1};

        for(int i=0; i<4; i++) {
            int nrow = row + delrow[i];
            int ncol = col + delcol[i];

            if (nrow >= 0 && nrow < n && ncol >= 0 && ncol < m &&
                !vis[nrow][ncol] && grid[nrow][ncol] == 1) {
                    dfs(grid, nrow, ncol, vis, island, rowBase, colBase);
                }
        }
}

int countDistinctIslands(vector<vector<int>> &grid) {
    int n = grid.size();
    int m = grid[0].size();

    vector<vector<int>> vis(n, vector<int>(m, 0));
    set<vector<pair<int, int>>> st;

    for(int i=0; i<n; i++) {
        for(int j=0; j<m; j++) {
            if(!vis[i][j] && grid[i][j] == 1) {
                vector<pair<int, int>> island;
                // n x m x 4
                dfs(grid, i, j, vis, island, i, j);
                st.insert(island);
            }
        }
    }
    // n x m x log(set len) + n x m x 4
    // n x m x log(n x m) + n x m x 4
    return st.size();
}

int main() {


    return 0;
}