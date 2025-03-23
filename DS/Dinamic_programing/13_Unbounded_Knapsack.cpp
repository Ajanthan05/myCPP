#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>

using namespace std;

/* Unbounded Knapsack: Infinite supplay
TC = exponential > O(2^N) boc we are standing at same place
SC = O(W) => bag weight as Axulary Stack Space
*/
int fun(int ind, int w, vector<int> &val, vector<int> &wt) {

    if (ind == 0) {
        // return (w / wt[0] >= 0) ? ((w / wt[0])*val[0]) : 0;
        return (wt[0] <= w) ? ((w / wt[0])*val[0]) : 0;
    }

    int notTake = fun(ind-1, w, val, wt);
    int take = INT_MIN;
    if (w >= wt[ind]) take = val[ind] + fun(ind, w-wt[ind], val, wt);

    return max(notTake, take);
}

int knapsackRec(int W, vector<int> &val, vector<int> &wt, int n) {
    return fun(n-1, W, val, wt);
}

int knapsackRec_TABULATION_SO(int W, vector<int> &val, vector<int> &wt, int n) {
    vector<int> prev(W+1, 0), cur(W+1, 0);
    for(int w=0; w<=W; w++) {
        prev[w] = ((int)(w / wt[0])) * val[0];
    }

    for(int ind=0; ind<n; ind++) {
        for(int w=0; w<=W; w++) {
            int notTake = 0 + prev[w];
            int take = 0;
            if (wt[ind] <= w) {
                take = val[ind] + cur[w - wt[ind]];
            }
            cur[w] = max(take, notTake);
        }
        prev = cur;
    }
    return prev[W];
}

/* Space optimize to 1D */
int knapsackRec_TABULATION_SO(int W, vector<int> &val, vector<int> &wt, int n) {
    vector<int> prev(W+1, 0); //cur(W+1, 0);
    for(int w=0; w<=W; w++) {
        prev[w] = ((int)(w / wt[0])) * val[0];
    }

    for(int ind=0; ind<n; ind++) {
        for(int w=0; w<=W; w++) {
            int notTake = 0 + prev[w];
            int take = 0;
            if (wt[ind] <= w) {
                take = val[ind] + prev[w - wt[ind]];
            }
            prev[w] = max(take, notTake);
        }
        // prev = cur;
    }
    return prev[W];
}


int main() {


    return 0;
}