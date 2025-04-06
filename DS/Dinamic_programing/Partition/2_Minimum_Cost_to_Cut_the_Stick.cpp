#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>

using namespace std;

int f(int i, int j, vector<int>& cuts) {

    if (i > j) return 0;

    int minCost = INT_MAX;
    for(int ind=i; ind<=j; ind++) {
        int cost = cuts[j+1] - cuts[i-1] + f(i, ind-1, cuts) + f(ind+1, j, cuts);
        minCost = min(minCost, cost);
    }
    return minCost;
}

int minCost(int n, vector<int>& cuts) {
    
    cuts.push_back(n);
    cuts.insert(cuts.begin(), 0);
    sort(cuts.begin(), cuts.end());
    int c = cuts.size();
    return f(1, c - 2, cuts); // ****
}

/* Memorization 
TC = O(N^2) * M = O(M^3)
SC = O(M^2) + Auxalary
*/
int f(int i, int j, vector<int>& cuts, vector<vector<int>> &dp) {

    if (i > j) return 0;

    if (dp[i][j] != -1) return dp[i][j];

    int minCost = INT_MAX;
    for(int ind=i; ind<=j; ind++) {
        int cost = cuts[j+1] - cuts[i-1] + f(i, ind-1, cuts, dp) + f(ind+1, j, cuts, dp);
        minCost = min(minCost, cost);
    }
    return dp[i][j] = minCost;
}

int minCost(int n, vector<int>& cuts) {
    
    cuts.push_back(n);
    cuts.insert(cuts.begin(), 0);
    sort(cuts.begin(), cuts.end());
    int c = cuts.size();

    vector<vector<int>> dp(c, vector<int>(c, -1));
    return f(1, c - 2, cuts, dp); // ****
}

/* TABULATION */
int minCost(int n, vector<int>& cuts) {
    
    cuts.push_back(n);
    cuts.insert(cuts.begin(), 0);
    sort(cuts.begin(), cuts.end());
    int c = cuts.size();

    vector<vector<int>> dp(c, vector<int>(c, 0));
    for(int i=c-2; i>=1; i--) {
        for(int j=i; j<=c-2; j++) {
            // if (i > j) {
            int minCost = INT_MAX;
            for(int ind=i; ind<=j; ind++) {
                int cost = cuts[j+1] - cuts[i-1] + dp[i][ind-1] + dp[ind+1][j];
                minCost = min(minCost, cost);
            }
            dp[i][j] = minCost;
        }
    }
    return dp[1][c-2];
}

int main() {
    vector<int> cuts = {1, 3, 4, 5};
    int n = 7;
    cout << "Minimum Cost: " << minCost(n, cuts) << endl;
    return 0;
}