#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>

using namespace std;
/*
TC = O(V * E)
SC = O(V)

distra TC = O(E * logV)
*/
vector<int> bellman_ford(int v, vector<vector<int>> &edges, int s) {
    vector<int> dist(v, 1e8);
    dist[s] = 0;
    for(int i=0; i < v - 1; i++) {
        for(auto it : edges) {
            int u = it[0];
            int v = it[1];
            int wt = it[2];
            if (dist[u] != 1e8 && dist[u] + wt < dist[v]) {
                dist[v] = dist[u] + wt;
            }
        }
    }

    // Nth relatation to check -ve cycle
    for(auto it : edges) {
        int u = it[0];
        int v = it[1];
        int wt = it[2];
        if (dist[u] != 1e8 && dist[u] + wt < dist[v]) {
            return {-1};
        }
    }
    return dist;
}

int main() {


    return 0;
}