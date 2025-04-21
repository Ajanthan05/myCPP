#include <iostream>
#include <vector>
#include <queue>

using namespace std;

/*
TC = O(N + 2E);  2E = twise of edge
every node we triverse all it adjesent nodes = degrees = 2E undirected
*/
bool detect(int src, vector<int> adj[], int vis[]) {
    vis[src] = 1;
    queue<pair<int, int>> q;
    q.push({src, -1});

    while(!q.empty()) {
        int node = q.front().first;
        int parent = q.front().second;
        q.pop();

        for(auto adjacentNode : adj[node]) {
            if (!vis[adjacentNode]) {
                vis[adjacentNode] = 1;
                q.push({adjacentNode, node});
            }
            // If it's been visited but not came from parent => boc it's cycle
            else if (parent != adjacentNode) {
                return true;
            }
        }
    }
}

bool isCycle(int v, vector<int> adj[]) {
    int vis[v] = {0};
    for(int i=0; i<v; i++) {
        if(!vis[i]) {
            if(detect(i, adj, vis)) return true;
        }
    }
    return false;
}

// Detect a Cycle in an Undirected Graph using DFS
/*
TC = O(N + 2E) + O(N)
SC = O(N) + O(N)
*/
bool DFS(int node, int parent, vector<int> adj[], int vis[]) {
    vis[node] = 1;

    for(auto adjacentNode : adj[node]) {
        if(!vis[adjacentNode]) {
            if (DFS(adjacentNode, node, adj, vis) == true) return true;
        }
        else if(adjacentNode != parent) return true;
    }
    return false;
}

bool isCycle_DFS(int v, vector<int> adj[]) {
    int vis[v] = {0};
    for(int i=0; i<v; i++) {
        if(!vis[i]) {
            if (DFS(i, -1, adj, vis) == true) return true;
        }
    }
    return false;
}

int main() {


    return 0;
}