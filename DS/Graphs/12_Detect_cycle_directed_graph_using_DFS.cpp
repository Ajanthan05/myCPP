#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>

using namespace std;

bool dfs(int node, vector<vector<int>> &adj, int vis[], int pathVis[]) {
    vis[node] = 1;
    pathVis[node] = 1;

    // Traverse for adjacent nodes
    for(auto it :adj[node]) {
        // when the node is not visited
        if (!vis[it]) {
            if(dfs(it, adj, vis, pathVis) == true)
                return true;
        }
        // If the node hasbeen previously visited
        // But it has to be visited on the same path
        else if (pathVis[it]) {
            return true;
        }
    }

    pathVis[node] = 0;
    return false;
}

bool isCyclic(vector<vector<int>> &adj) {
    int n = adj.size();
    int vis[n] = {0};
    int pathVis[n] = {0};

    for(int i=0; i<n; i++) {
        if(!vis[i]) {
            if (dfs(i, adj, vis, pathVis) == true) return true;
        }
    }
    return false;
}

int main() {

    int V = 4;
    vector<vector<int>> adj(V);

    // Adding edges to the graph
    adj[0].push_back(1);
    adj[0].push_back(2);
    adj[1].push_back(2);
    adj[2].push_back(0);
    adj[2].push_back(3);
    adj[3].push_back(3);

    // Function call
    if (isCyclic(adj))
        cout << "Contains Cycle" << endl;
    else
        cout << "No Cycle" << endl;

    return 0;
}