#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <algorithm>

using namespace std;

/* If land is connected to your boundry it never be an answer

Find boundry once and clnnect all the ones are connected

remaining ones are answer
*/
int numEnclaves(vector<vector<int>>& grid) {
    int n = grid.size();
    int m = grid[0].size();
    
    // Fix: Use vector instead of VLA
    vector<vector<int>> vis(n, vector<int>(m, 0));
    queue<pair<int, int>> q;

    // Mark boundary cells
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if ((i == 0 || i == n - 1 || j == 0 || j == m - 1) && grid[i][j] == 1) { // Fix: Only process land cells
                q.push({i, j});
                vis[i][j] = 1;
            }
        }
    }

    // Direction vectors
    int delrow[] = {-1, 0, 1, 0};
    int delcol[] = {0, 1, 0, -1};

    // BFS Traversal
    while (!q.empty()) {
        int row = q.front().first;
        int col = q.front().second;
        q.pop();

        for (int i = 0; i < 4; i++) {
            int nrow = row + delrow[i];
            int ncol = col + delcol[i];

            if (nrow >= 0 && nrow < n && ncol >= 0 && ncol < m 
                && vis[nrow][ncol] == 0 && grid[nrow][ncol] == 1) {  // Fix: Correct row & col indices
                q.push({nrow, ncol});
                vis[nrow][ncol] = 1;
            }
        }
    }

    // Count remaining land cells that are not visited
    int cnt = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (grid[i][j] == 1 && vis[i][j] == 0) {
                cnt++;
            }
        }
    }
    return cnt;
}



int numEnclaves2(vector<vector<int>>& grid) {
    int n = grid.size(), m = grid[0].size();
    queue<pair<int, int>> q;
    
    // Push all boundary land cells into queue and mark them as visited (turning 1 → 0)
    for (int i = 0; i < n; i++) {
        for (int j : {0, m - 1}) {
            if (grid[i][j] == 1) {
                q.push({i, j});
                grid[i][j] = 0;
            }
        }
    }
    for (int j = 0; j < m; j++) {
        for (int i : {0, n - 1}) {
            if (grid[i][j] == 1) {
                q.push({i, j});
                grid[i][j] = 0;
            }
        }
    }

    // Directions for 4-way movement
    int delrow[] = {-1, 0, 1, 0};
    int delcol[] = {0, 1, 0, -1};

    // BFS traversal to remove all connected land cells
    while (!q.empty()) {
        auto [row, col] = q.front();
        q.pop();
        for (int i = 0; i < 4; i++) {
            int nrow = row + delrow[i], ncol = col + delcol[i];
            if (nrow >= 0 && nrow < n && ncol >= 0 && ncol < m && grid[nrow][ncol] == 1) {
                q.push({nrow, ncol});
                grid[nrow][ncol] = 0;
            }
        }
    }

    // Count remaining land cells
    int cnt = 0;
    for (const auto& row : grid) {
        cnt += count(row.begin(), row.end(), 1);
    }
    
    return cnt;
}


int main() {


    return 0;
}