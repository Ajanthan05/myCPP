#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>

using namespace std;

void dfs(int node, vector<int> &vis, vector<int> adj[], stack<int> &st) {
    vis[node] = 1;
    for(auto it : adj[node]) {
        if (!vis[it]) {
            dfs(it , vis, adj, st);
        }
    }
    st.push(node);
}

void dfs3(int node, vector<int> &vis, vector<int> adj[]) {
    vis[node] = 1;
    for(auto it : adj[node]) {
        if (!vis[it]) {
            dfs3(it , vis, adj);
        }
    }
}

int Kosarsju(int v, vector<int> adj[]) {
    vector<int> vis(v, 0);
    stack<int> st;
    // DFS take O(E+V)
    for(int i=0; i<v; i++) {
        if(!vis[i]) {
            dfs(i, vis, adj, st);
        }
    }

    vector<int> adjT[v];
    for(int i=0; i<v; i++) {
        vis[i] = 0;
        for(auto it : adj[i]) {
            // i -> it
            // it - > i
            adjT[it].push_back(i);
        }
    }

    int scc = 0;
    while(!st.empty()) {
        int node = st.top();
        st.pop();
        if (!vis[node]) {
            scc++;
            dfs3(node, vis, adjT);
        }
    }
    return scc;
}

int main() {


    return 0;
}