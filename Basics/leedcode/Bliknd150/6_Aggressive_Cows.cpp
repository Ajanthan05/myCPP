#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>

using namespace std;

int aggressiveCows(vector<int> &stalls, int k) {
    int n = stalls.size();
    int low = 0;
    int high = stalls[n-1];

    while(low <= high) {
        int mid = low/2 + high/2;  //high + (low-high)/2;
        
    }
}


int main() {


    return 0;
}