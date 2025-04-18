#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>

using namespace std;
/* Dijkstras wont work which have negative cycle 
Although it take less time TC = N * ElogV

TC = N^3
*/
void shortest_distance(vector<vector<int>> &matrix) {
    int n = matrix.size();
    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
            if (matrix[i][j] == -1) {
                matrix[i][j] = 1e9;
            }
            if (i == j) matrix[i][j] = 0;
        }
    }

    for(int k=0; k<n; k++) {
        for(int i=0; i<n; i++) {
            for(int j=0; j<n; j++) {
                matrix[i][j] = min(matrix[i][j], matrix[i][k] + matrix[k][j]);
            }
        }
    }

    for(int i=0; i<n; i++) {
        if(matrix[i][i] < 0) {
            cout << "Negative Cycle\n";
        }
    }

    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
            if (matrix[i][j] == 1e9) {
                matrix[i][j] = -1;
            }
        }
    }
}

int main() {


    return 0;
}