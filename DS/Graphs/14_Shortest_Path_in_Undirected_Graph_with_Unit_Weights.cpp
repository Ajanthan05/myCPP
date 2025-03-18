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

vector<int> shortestPath(vector<vector<int>>& edges, int src) {
    int n = edges.size();
    vector<vector<int>> adj(n);

    for(auto it : edges) {
        adj[it[0]].push_back(it[1]);
        // Undirected graph have both side edges
        adj[it[1]].push_back(it[0]);
    }

    vector<int> dist(n);
    std::fill(dist.begin(), dist.end(), 1e9);
    dist[src] = 0;

    queue<int> q;
    q.push(src);

    while(!q.empty()) {
        int node = q.front();
        q.pop();
        for(auto it : adj[node]) {
            if(dist[node] + 1 < dist[it]) {
                dist[it] = dist[node] + 1;
                q.push(it);
            } 
        }
    }

    vector<int> ans(n, -1);
    for(int i=0; i<n; i++) {
        if(dist[i] != 1e9) {
            ans[i] = dist[i];
        }
    }
    return ans;
}

int main() {


    return 0;
}