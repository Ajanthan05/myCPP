#include <iostream>
#include <vector>
#include <queue>
#include <pair>
#include <utility>

using namespace std;

/* BFS must boc we need to travel 1 stap at a time
We want traverse all of them, one step at a time
*/
vector<vector<int>> fun(vector<vector<int>> &mat, vector<vector<int>> &dis, vector<vector<int>> &vis) {

}

vector<vector<int>> updateMatrix(vector<vector<int>>& mat) {
    vector<vector<int>> vis;
    vector<vector<int>> dis;

    int n = mat.size();
    int m = mat[0].size();
    queue<pair<pair<int, int>, int>> q;

    for(int i=0; i<n; i++) {
        for(int j=0; i<m; i++) {
            if (mat[i][j] == 1) {
                q.push({{i,j},0});
                vis[i][j] = 1;
            }
            else {
                vis[i][j] = 0;
            }
        }
    }

    int delrow = {-1,  0,  1,  0};
    int delcol = { 0,  1,  0, -1};

    while(!q.empty()) {
        int row = q.front().first.first;
        int col = q.front().first.second;
        int steps = q.front().second;
        q.pop();
        dist[row][col] = steps;

        for(int i=0; i<4; ++i) {
            int nrow = row + delrow[i];
            int ncol = col + delcol[i];

            if (nrow >= 0 && nrow < n && ncol >= 0 && ncol < m 
            && vis[nrow][ncol] == 0) {
                vis[nrow][ncol] == 1;
                q.
            }
        }

    }
}

int main() {


    return 0;
}