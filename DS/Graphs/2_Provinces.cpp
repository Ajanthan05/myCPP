#include <iostream>
#include <vector>
#include <queue>

// void bfs(int start, std::vector<int> adj[], int vis) {
//     // vis[start] = 1;S
//     // std::queue<int> = q;

// }
// void bfs(int start, std::vector<int> adj[], std::vector<int>& vis) { // BFS implementation
void bfs(int start, std::vector<int> adj[], int vis[]) { // BFS implementation
    std::queue<int> q;
    q.push(start);
    vis[start] = 1;

    while (!q.empty()) {
        int node = q.front();
        q.pop();

        for (auto it : adj[node]) {
            if (!vis[it]) {
                vis[it] = 1;
                q.push(it);
            }
        }
    }
}

void dfs(int node, std::vector<int> adj[], std::vector<int> &vis) {
    vis[node] = 1;
    for(auto it : adj[node]) {
        if(!vis[it]) {
            dfs(it, adj, vis);
        }
    }
}

int findCircleNum(std::vector<std::vector<int>>& isConnected) {
    // Adjency 
    int v = isConnected.size();
    std::vector<int> adj[v];
    for(int i=0; i<v; i++) {
        for(int j=0; j<v; j++) {
            if(isConnected[i][j] == 1 && i != j) {
                adj[i].push_back(j);
                adj[j].push_back(i);
            }
        }
    }

    // std::vector<int> vis(v, 0);    // int vis[v] = {0};
    int vis[v] = {0};
    int cnt = 0;

    for(int i=0; i<v; i++) {
        if(!vis[i]) {
            cnt++;
            // dfs(i, adj, vis); // OR
            bfs(i, adj, vis);
        }
    }
    return cnt;
}

int main() {

    std::vector<std::vector<int>> isConnected = {{1,1,0},{1,1,0},{0,0,1}};
    std::cout << findCircleNum(isConnected) << "\n";

    std::vector<std::vector<int>> isConnected2 = {{1,0,0},{0,1,0},{0,0,1}};
    std::cout << findCircleNum(isConnected2) << "\n";
    return 0;
}