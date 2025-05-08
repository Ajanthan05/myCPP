#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>

using namespace std;

int findCelebrity(vector<vector<int>> vec, int n) {
    vector<int> KnowMe(n);
    vector<int> IKnow(n);
    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
            if(vec[i][j] == 1) {
                KnowMe[j]++;
                IKnow[i]++;
            }
        }
    }

    for(int i=0; i<n; i++) {
        if (IKnow[i] == 0 && KnowMe[i] == n-1) return i;
    }
    return -1;
}

int findCelebrity_SO(vector<vector<int>> vec, int n) {
    int top = 0;
    int down = n-1;
    while(top < down) {
        if(vec[top][down] == 1) top++;
        else if(vec[down][top]==1) down--;
        else {
            top++;
            down--;
        }
    }
    if (top > down) return -1;
    for(int i=0; i<n; i++) {
        if(top != i && (vec[top][i] != 0 || vec[i][top] != 1)) return -1;
    }
    return top;
}


int main() {
    vector<vector<int>> vec = { {0,1,1,0},
                                {0,0,0,0},
                                {0,1,0,0},
                                {1,1,0,0}};

    cout << findCelebrity_SO(vec, 4) << "\n";
    return 0;
}