#include <iostream>
#include <vector>
#include <climits>

using namespace std;

void dfs(vector<vector<int>> &ans, int row, int col, vector<vector<int>> &image, int newColor, int initialColor) {
    ans[row][col] = newColor;
    int n = image.size();
    int m = image[0].size();

    vector<pair<int, int>> direction = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
    for(auto [delRow, delCol] : direction) {
        int nrow = row + delRow;
        int ncol = col + delCol;

        if (nrow >= 0 && nrow < n && ncol >= 0 && ncol < m &&
        image[nrow][ncol] == initialColor && ans[nrow][ncol] != newColor) {
            dfs(ans, nrow, ncol, image, newColor, initialColor);
        }
     }
}

vector<vector<int>> floodFill(vector<vector<int>>& image, int sr, int sc, int color) {
    int initialColor = image[sr][sc];
    vector<vector<int>> ans = image;
    dfs(ans, sr, sc, image, color, initialColor);
    return ans;
}

int main() {


    return 0;
}