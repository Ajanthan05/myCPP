#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <utility>
#include <algorithm>

using namespace std;
/* Topological sorting is Linear Ordering of vertices */

void dfs(int node, int vis[], stack<int> &st, vector<vector<int>>& adj) {
    vis[node] = 1;
    for(auto it : adj[node]) {
        if(!vis[it]) dfs(it, vis, st, adj);
    }
    // Before going back put it in stack (Last In First Out)
    st.push(node);
}

vector<int> topologicalSort(vector<vector<int>>& adj) {
    int n = adj.size();
    int vis[n] = {0};

    stack<int> st;
    // run through all nodes (n)
    for(int i=0; i<n; i++) {
        if(!vis[i]) {
            dfs(i, vis, st, adj);
        }
    }

    // We are getting one of the linear ordering
    vector<int> ans(n);
    for(int i=0; i<n; i++) {
        ans.push_back(st.top());
        st.pop();
    }
    return ans;
}

/* KHAN'S ALGORITHM (BFS) */
vector<int> topologicalSort(vector<vector<int>>& adj) {
    int n = adj.size();
    
    int indegree[n] = {};
    for(int i=0; i<n; i++) {
        for(auto it : adj[i]) {
            indegree[it]++;
        }
    }

    queue<int> q;
    for(int i=0; i<n; i++) {
        if(indegree[i] == 0) q.push(i);
    }

    vector<int> topo;
    while(!q.empty()) {
        int node = q.front();
        q.pop();
        topo.push_back(node);
        
        // Node is in your topo sort
        // So please romove it from the indegree
        for(auto it : adj[node]) {
            indegree[it]--;
            if(indegree[it] == 0) q.push(it);
        }
    }
    return topo;
}

int main() {


    return 0;
}