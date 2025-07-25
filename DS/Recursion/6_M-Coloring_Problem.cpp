#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>

using namespace std;
/*
TC = O(N^M)
*/
bool possible(int node, vector<vector<int>> &mat, int col, vector<int> &color) {
    for (auto it : mat[node]) {
        if (color[it] == col) return false;
    }
    return true;
}

bool f(int node, vector<vector<int>> &mat, int m, int n, vector<int> &color) {
    if (node == n) return true;

    for (int col = 0; col < m; col++) {
        if (possible(node, mat, col, color)) {
            color[node] = col;
            if (f(node + 1, mat, m, n, color)) {
                return true;
            }
            color[node] = -1;  // Reset correctly
        }
    }
    return false;
}

string graphColoring(vector<vector<int>> &mat, int m) {
    int n = mat.size();
    vector<int> color(n, -1);  // Correct initialization
    return f(0, mat, m, n, color) ? "YES" : "NO";
}


int main() {
    // bool graph[101][101];
    vector<vector<int>> mat = {{0,1,0}, {1,0,1}, {0,1,0}};
    cout << graphColoring(mat, 3) << "\n";
    return 0;
}