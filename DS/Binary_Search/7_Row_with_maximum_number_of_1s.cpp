#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>

using namespace std;

int rowWithMax1s(vector<vector<int>> &matrix, int n, int m) {
    int ind = -1;
    int maxc = -1;

    for(int i=0; i<n; i++) {
        int cnt = -1;  // cnt = 0; problem {{0},{0}}
        for(int j=0; j<m; j++) {
            if(matrix[i][j] == 1) {
                cnt = m-j+1;
                break;
            }
        }
        if(cnt > maxc) {
            maxc = cnt;
            ind = i;
        }
    }
    return ind;
}

int main() {


    return 0;
}