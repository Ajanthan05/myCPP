#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>

using namespace std;

int trap(vector<int>& height) {
    int n = height.size();
    int lMax = 0, rMax = height[n-1];
    int l = 0, r = n-1;
    int total = 0;

    while(l < r) {
        if (height[l] <= height[r]) {
            if (lMax > height[l]) {
                total += lMax - height[l];
            }
            else {
                lMax = height[l];
            }
            l++;
        }
        else {
            if (rMax > height[r]) {
                total += rMax - height[r];
            }
            else {
                rMax = height[r];
            }
            r--;
        }
    }
    return total;
}

int trapRainWater(vector<vector<int>>& heightMap) {
    int n = heightMap.size();

}

int trapRainWater(vector<vector<int>>& heightMap) {
    int m = heightMap.size(), n = heightMap[0].size();
    if (m < 3 || n < 3) return 0; // No water can be trapped if it's less than 3x3

    priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, greater<>> pq;
    vector<vector<bool>> visited(m, vector<bool>(n, false));

    // Push all boundary cells into the min-heap and mark them as visited
    for (int i = 0; i < m; i++) {
        pq.push({heightMap[i][0], {i, 0}});
        pq.push({heightMap[i][n-1], {i, n-1}});
        visited[i][0] = visited[i][n-1] = true;
    }
    for (int j = 1; j < n-1; j++) {
        pq.push({heightMap[0][j], {0, j}});
        pq.push({heightMap[m-1][j], {m-1, j}});
        visited[0][j] = visited[m-1][j] = true;
    }

    int totalWater = 0;
    vector<vector<int>> directions = {{0,1}, {1,0}, {0,-1}, {-1,0}}; // Right, Down, Left, Up

    // Process the heap
    while (!pq.empty()) {
        auto [height, cell] = pq.top(); pq.pop();
        int x = cell.first, y = cell.second;

        for (auto& dir : directions) {
            int nx = x + dir[0], ny = y + dir[1];

            if (nx >= 0 && ny >= 0 && nx < m && ny < n && !visited[nx][ny]) {
                visited[nx][ny] = true;

                // If the neighboring cell is lower, it can trap water
                totalWater += max(0, height - heightMap[nx][ny]);

                // Update the boundary height to max(current boundary, neighbor's height)
                pq.push({max(height, heightMap[nx][ny]), {nx, ny}});
            }
        }
    }

    return totalWater;
}
/* This problem can be efficiently solved using a Min-Heap (Priority Queue) and BFS (Dijkstra-like approach). The key idea is to always process the lowest boundary first and gradually expand inward.

Approach:
Use a Min-Heap to always process the lowest height first.

Mark the boundary cells as visited and push them into the Min-Heap.

Process the lowest cell:

Try moving in all four directions (up, down, left, right).

If the new cell is lower than the current boundary, it can trap water.

The trapped water volume is determined by the difference between the boundary height and the new cell height.

Push the new cell into the heap with the updated boundary height.

Continue the process until all reachable cells are processed.
*/
int main() {


    return 0;
}