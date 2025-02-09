#include <iostream>
#include <vector>
#include <queue>
#include <tuple>

/*
struct tuple {
    int i, j;
    int steps;
    tuple(int _i, int _j, int _steps) : i(_i), j(_j), steps(_steps) {}
};

int orangesRotting1(std::vector<std::vector<int>>& grid) {
    int n = grid.size();
    int m = grid[0].size();

    int cntNonEmpty = 0;
    int rottenI = 0;
    int rottenJ = 0;

    for(int i=0; i<n; i++) {
        for(int j=0; j<m; j++) {
            if(grid[i][j] == 2) {
                rottenI = i;
                rottenJ = j;
                cntNonEmpty++;
            }
            else if(grid[i][j] == 1) {
                cntNonEmpty++;
            }
        }
    }

    std::queue<tuple> q;
    // q.push(new tuple(rottenI, rottenJ, 0));
    std::vector<std::vector<int>> vis(n, std::vector<int>(m, 0));
    vis[rottenI][rottenJ] = 1;
    int cnt = 0;
    int maxT = 0;

    while(!q.empty()) {
        auto it = q.front();
        q.pop();
        cnt++;

        int i = it.i;
        int j = it.j;
        int steps = it.steps;
        maxT = std::max(maxT, steps);

        // Insert all the 4 direction
    }

    if(cnt == cntNonEmpty) return maxT;
    else return -1;
}




int orangesRotting_old(std::vector<std::vector<int>>& grid) {
    int n = grid.size();
    int m = grid[0].size();
    std::queue<std::tuple<int, int, int>> q; // {i, j, time}
    int freshCount = 0;
    int maxTime = 0;

    // Directions for BFS traversal (right, left, down, up)
    std::vector<std::pair<int, int>> directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

    // Initialize queue with all rotten oranges and count fresh oranges
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (grid[i][j] == 2) {
                q.push({i, j, 0}); // Push rotten orange with time 0
            } else if (grid[i][j] == 1) {
                freshCount++;
            }
        }
    }

    // BFS traversal
    while (!q.empty()) {
        auto [i, j, time] = q.front();
        q.pop();
        maxTime = std::max(maxTime, time);

        for (auto [di, dj] : directions) {
            int ni = i + di;
            int nj = j + dj;

            // Check bounds and if it's a fresh orange
            if (ni >= 0 && ni < n && nj >= 0 && nj < m && grid[ni][nj] == 1) {
                grid[ni][nj] = 2; // Mark as rotten
                freshCount--;     // Decrease fresh count
                q.push({ni, nj, time + 1});
            }
        }
    }

    // If any fresh oranges remain, return -1
    return (freshCount == 0) ? maxTime : -1;
}
*/

int orangesRotting(std::vector<std::vector<int>>& grid) {
    int n = grid.size();
    int m = grid[0].size();
    int freshCount = 0;
    int maxTime = 0;

    std::queue<std::tuple<int, int, int>> q; // {i, j, time}

    std::vector<std::pair<int, int>> directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
 
    std::vector<std::vector<int>> vis(n, std::vector<int>(m, 0));

    for(int i=0; i<n; i++) {
        for(int j=0; j<m; j++) {
            if(grid[i][j] == 2) {
                q.push({i, j, 0});
                vis[i][j] = 1;
            } 
            else if(grid[i][j] == 1) {
                freshCount++;
            }
        }
    }

    while(!q.empty()) {
        auto [i, j, time] = q.front();
        q.pop();
        maxTime = std::max(maxTime, time);

        for(auto [di, dj] : directions) {
            int ni = i + di;
            int nj = j + dj;

            if(ni >= 0 && ni < n && nj >= 0 && nj < m && grid[ni][nj] == 1 && vis[ni][nj] == 0) {
                vis[ni][nj] = 1;
                freshCount--;
                q.push({ni, nj, time + 1});
            }
        }
    }

    return (freshCount == 0) ? maxTime : -1;
}

int countBattleships(std::vector<std::vector<char>>& board) {
    int n = board.size();
    int m = board[0].size();
    int freshCount = 0;
    int maxTime = 0;

    std::queue<std::tuple<int, int, int>> q; // {i, j, time}

    std::vector<std::pair<int, int>> directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
 
    std::vector<std::vector<int>> vis(n, std::vector<int>(m, 0));

    for(int i=0; i<n; i++) {
        for(int j=0; j<m; j++) {
            if(board[i][j] == 'X') {
                q.push({i, j, 0});
                vis[i][j] = 1;
            } 
            else if(board[i][j] == 1) {
                freshCount++;
            }
        }
    }

    while(!q.empty()) {
        auto [i, j, time] = q.front();
        q.pop();
        maxTime = std::max(maxTime, time);

        for(auto [di, dj] : directions) {
            int ni = i + di;
            int nj = j + dj;

            if(ni >= 0 && ni < n && nj >= 0 && nj < m && board[ni][nj] == 1 && vis[ni][nj] == 0) {
                vis[ni][nj] = 1;
                freshCount--;
                q.push({ni, nj, time + 1});
            }
        }
    }

    return (freshCount == 0) ? maxTime : -1;
}

int main() {
    std::vector<std::vector<int>> grid = {
        {2, 1, 1},
        {1, 1, 0},
        {0, 1, 1}
    };

    std::cout << "Time taken to rot all oranges: " << orangesRotting(grid) << std::endl;
    return 0;
}

