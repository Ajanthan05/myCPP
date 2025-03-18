#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <utility>
#include <algorithm>

using namespace std;

void topoSort(int node, vector<vector<pair<int, int>>> &adj, int vis[], stack<int> &st) {
    vis[node] = 1;
    for(auto it :adj[node]) {
        int v = it.first;
        if(!vis[v]) {
            topoSort(v, adj, vis, st);
        }
    }

    st.push(node);
}

vector<int> sortestPath(vector<vector<int>> edges, vector<vector<pair<int, int>>> adj) {
    int n = edges.size();

    for(int i=0; i<n; i++) {
        int u = edges[i][0];
        int v = edges[i][1];
        int wt = edges[i][2];
        adj[u].push_back({v, wt});
    }

    int vis[n] = {0};
    stack<int> st;
    for(int i=0; i<n; i++) {
        if(!vis[i]) {
            topoSort(i, adj , vis, st);
        }
    }

    // Step2 do the distance thing
}

int main() {


    return 0;
}