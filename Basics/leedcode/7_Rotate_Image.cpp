#include <iostream>
#include <vector>
#include <climits>

void Reverse(vector<int> &arr) {
    int p1 = 0, p2 = arr.size() - 1;
    while(p1 < p2) {
        std::swap(arr[p1], arr[p2]);
        p1++; p2--;
    }
}

void rotate(vector<vector<int>>& matrix) {
    int n = matrix.size();
    for (int i=0; i<=n-2; i++) {   // Go up to n-2
        for (int j=i+1; j<n; j++) {
            std::swap(matrix[i][j], matrix[j][i]);
        }
    }

    for (int i=0; i<n; i++) {
        std::reverse(matrix[i].begin(), matrix[i].end());
    }
}

int main() {


    return 0;
}