#include <iostream>
#include <vector>
#include <queue>

using namespace std;

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

int main() {


    return 0;
}