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

/* KHAN'S ALGORITHM (BFS) 
INDEGREE :- no of incoming edges to a node
*/
vector<int> topologicalSort(vector<vector<int>>& adj) {
    int n = adj.size();
    
    int indegree[n] = {};
    for(int i=0; i<n; i++) {
        for(auto it : adj[i]) {
            indegree[it]++;
        }
    }

    // Insert all nodes with indegree 0
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

/*  Detect a Cycle in Directed Graph | Topological Sort */
bool isCyclic(vector<vector<int>> &adj) {
    int n = adj.size();
    int indegree[n] = {};

    for(int i=0; i<n; i++) {
        for(auto it : adj[i]) {
            indegree[it]++;
        }
    }

    queue<int> q;
    for(int i=0; i<n; i++) {
        if (indegree[i] == 0) q.push(i);
    }

    vector<int> topo;
    while(!q.empty()) {
        int node = q.front();
        q.pop();
        topo.push_back(node);

        for(auto it : adj[node]) {
            indegree[it]--;
            if (indegree[it] == 0) q.push(it);
        }
    }
    return (topo.size() == n);
}


/* Course Schedule
There are a total of numCourses courses you have to take, labeled from 0 to numCourses - 1. 
You are given an array prerequisites where prerequisites[i] = [ai, bi] indicates that you must \
take course bi first if you want to take course ai.*/
bool canFinish(int numCourses, vector<vector<int>>& prerequisites) {
    vector<int> indegree(numCourses, 0);
    vector<vector<int>> adj(numCourses);

    // Construct adjacency list and compute in-degrees
    for(auto& pre : prerequisites) {
        int u = pre[1]; // Course `b` (prerequisite)
        int v = pre[0]; // Course `a` (depends on `b`)
        adj[u].push_back(v);
        indegree[v]++;
    }

    // Push all nodes with 0 in-degree to queue
    queue<int> q;
    for(int i = 0; i < numCourses; i++) {
        if (indegree[i] == 0) q.push(i);
    }

    int count = 0; // Track number of processed courses
    while(!q.empty()) {
        int node = q.front();
        q.pop();
        count++;

        for(auto& neighbor : adj[node]) {
            indegree[neighbor]--;
            if (indegree[neighbor] == 0) q.push(neighbor);
        }
    }

    return count == numCourses;
}

/*
*/
vector<int> eventualSafeNodes(vector<vector<int>>& graph) {
    int n = graph.size();
    vector<int> indegree(n, 0);

    // i -> it
    // it -> i  (it goes to i)
    vector<vector<int>> adjRev(n);
    for(int i=0; i<n; i++) {
        for(auto it :  graph[i]) {
            adjRev[it].push_back(i);
            indegree[i]++;
        }
    }

    queue<int> q;
    for(int i=0; i<n; i++) {
        if (indegree[i] == 0) {
            q.push(i);
        }
    }

    vector<int> safeNodes;
    while(!q.empty()) {
        int node = q.front();
        q.pop();
        safeNodes.push_back(node);
        for(auto it : adjRev[node]) {
            indegree[it]--;
            if(indegree[it] == 0) q.push(it);
        }
    }

    sort(safeNodes.begin(), safeNodes.end());
    return safeNodes;
}

int main() {


    return 0;
}