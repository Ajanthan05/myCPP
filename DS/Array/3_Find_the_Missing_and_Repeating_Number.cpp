#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>

using namespace std;

vector<int> findMissingAndRepeatedValues(vector<vector<int>>& grid) {
    int n = grid.size();
    // int hash[n*n + 1]= {0};
    int total = n*n;
    vector<int> hash(total + 1, 0);
    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
            hash[grid[i][j]]++;
        }
    }

    int repeating = -1;
    int missing = -1;
    for(int i=1; i<=total; i++) {
        if(hash[i] == 0) missing = i;
        if(hash[i] == 2) repeating = i;

        if(missing != -1 && repeating != -1) break;
    }
    return {repeating, missing};
}


/*
x = repeating
y = missing

Sn = n(N+1)/2
S2n = n(n+1)(2n+1)/6

S - Sn = ans1 = x-y
s^2 -Sn^2 = ans2 = (x-y)(x+y)
*/
vector<int> findMissingAndRepeatedValues(vector<vector<int>>& grid) {
    int n = grid.size();
    int t = n*n;
    long long Sn = t * (t + 1) / 2;
    long long s2n = t * (t+1) * (2*t + 1) / 6;

    int s=0 , s2=0;
    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
            s  += grid[i][j];
            s2 += grid[i][j] * grid[i][j];
        }
    }
    int xsuby = s - Sn;
    int sum2xy = s2 - s2n;
    int xsumy = sum2xy / xsuby;

    int repeating = (xsumy + xsuby)/2;
    int missing = xsumy - repeating;
    return {repeating, missing};
}


/* Correction*/
vector<int> findMissingAndRepeatedValues(vector<vector<int>>& grid) {
    int n = grid.size();
    int t = n * n;

    long long Sn = t * (t + 1LL) / 2;                  // Sum of first t natural numbers
    long long s2n = t * (t + 1LL) * (2 * t + 1LL) / 6; // Sum of squares of first t natural numbers

    long long s = 0, s2 = 0;  // Use long long to store sum values

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            long long val = grid[i][j];  // Convert to long long before operations
            s  += val;
            s2 += val * val;  // Ensure correct multiplication without overflow
        }
    }

    long long xsuby = Sn - s;      // missing - repeating
    long long sum2xy = s2n - s2;   // missing^2 - repeating^2

    long long xsumy = sum2xy / xsuby; // (missing + repeating)

    int repeating = (xsumy - xsuby) / 2;
    int missing = xsumy - repeating;

    return {repeating, missing};
}

int main() {


    return 0;
}