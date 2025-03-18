#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>

using namespace std;

/* NOT  All the rows are sorted in ascending order*/
vector<int> rowAndMaximumOnes(vector<vector<int>>& mat) {
    int maxc = 0, ind = 0;
    
    for (int i = 0; i < mat.size(); i++) {
        int cnt = count(mat[i].begin(), mat[i].end(), 1);  // STL count function
        if (cnt > maxc) {
            maxc = cnt;
            ind = i;
        }
    }
    
    return {ind, maxc};
}


/*  All the rows are sorted in ascending order */
int rowWithMax1s(vector<vector<int>> &matrix, int n, int m) {
    int ind = -1;
    int maxc = -1;

    for(int i=0; i<n; i++) {
        int cnt = -1;  // cnt = 0; problem when {{0},{0}}
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

int rowWithMax1s_BST(vector<vector<int>> &matrix, int n, int m) {

}


int main() {


    return 0;
}