#include <iostream>
#include <vector>
#include <queue>
// #include <pair>

using namespace std;
/*
void bfs(int row, int col, vector<vector<int>> &vis, vector<vector<char>> &grid, int n, int m) {
    vis[row][col] = 1;
    queue<pair<int, int>> q;
    q.push({row, col});

    while(!q.empty()) {
        int row = q.front().first;
        int col = q.front().second;
        q.pop();

        // traverse in a neighbours and mark them if its is a land
        for(int delrow=-1; delrow<=1; delrow++) {
            for(int delcol=-1; delcol<=1; delcol++) {
                int nrow = row + delrow;
                int ncol = col + delcol;

                if ( nrow >= 0 && nrow < n && ncol >= 0 && ncol < m 
                && grid[nrow][ncol] == '1' && !vis[nrow][ncol]) {
                    vis[nrow][ncol] = 1;
                    q.push({nrow, ncol});
                }
            }
        }
    }
}

int numIslands(vector<vector<char>>& grid) {
    int n = grid.size();
    int m = grid[0].size();
    int cnt = 0;
    vector<vector<int>> vis(n, vector<int>(m, 0));

    for (int row=0; row<n; row++) {
        for(int col=0; col<m; col++) {
            if (!vis[row][col] && grid[row][col] == '1') {
                cnt++;
                bfs(row, col, vis, grid, n, m);
            }
        }
    }
    return cnt;
}

*/

void bfs(int row, int col, vector<vector<int>> &vis, vector<vector<char>> &grid, int n, int m) {
    vis[row][col] = 1;
    queue<pair<int, int>> q;
    q.push({row, col});

    // Directions for 4-neighbor traversal (up, down, left, right)
    // vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    // Directions for 8-neighbor traversal
    vector<pair<int, int>> directions = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}, // Up, Down, Left, Right
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1} // Diagonal directions
    };

    while (!q.empty()) {
        int row = q.front().first;
        int col = q.front().second;
        q.pop();

        for (auto [dr, dc] : directions) {
            int nrow = row + dr;
            int ncol = col + dc;

            if (nrow >= 0 && nrow < n && ncol >= 0 && ncol < m 
                && grid[nrow][ncol] == '1' && !vis[nrow][ncol]) {
                vis[nrow][ncol] = 1;
                q.push({nrow, ncol});
            }
        }
    }
}

int numIslands(vector<vector<char>>& grid) {
    int n = grid.size();
    int m = grid[0].size();
    int cnt = 0;
    vector<vector<int>> vis(n, vector<int>(m, 0));

    for (int row = 0; row < n; row++) {
        for (int col = 0; col < m; col++) {
            if (!vis[row][col] && grid[row][col] == '1') {
                cnt++;
                bfs(row, col, vis, grid, n, m);
            }
        }
    }
    return cnt;
}

int main() {


    return 0;
}