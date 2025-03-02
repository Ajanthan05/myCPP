#include <iostream>
#include <vector>
#include <queue>
// #include <pair>
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
            if (mat[i][j] == 0) {
                q.push({{i,j},0});
                dis[i][j] = 0;
            }
            else {
                vis[i][j] = 0;
            }
        }
    }
}

int main() {


    return 0;
}