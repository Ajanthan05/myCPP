#include <iostream>
#include <vector>
#include <queue>
#include <climits>

/*
WHEN A NODE ENTER IT RUNS ALL IT'S NO OF NEIGHBOUR NODES(  Degrees)
TC = O(N) + O(2E)  // WHILE LOOP + ITS TOTAL DEGREES
SC = O(3N)
*/
std::vector<int> bfsOfGraph(int v, std::vector<int> adj[]) {
    int vis[v] = {0}; // Zero base indexing
    vis[0] = 1;  // 1 is the head 
    std::queue<int> q;
    q.push(0);
    std::vector<int> bfs;

    while(!q.empty()) {
        int node = q.front();
        q.pop();
        bfs.push_back(node);

        for(auto it : adj[node]) {
            if(!vis[it]) {
                vis[it] = 1;
                q.push(it);
            }
        }
    }
    return bfs;
}

std::vector<int> dfs(int node, std::vector<int> adj[], ins vis[], std::vector<int> &ls) {
    vis[node] = 1;
    ls.push_back(node);

    for(auto it : adj[node]) {
        if(!vis[it]) {
            dfs(it, adj, vis, ls);
        }
    }
}

std::vector<int> dfsOfGraph(int v, std::vector<int> adj[]) {
    int vis[v] = {0};
    int start = 0;
    std::vector<int> ls;
    dfs(start, adj, vis, ls);
    return ls;
}

int main() {
    int v = 5; // Number of vertices
    std::vector<int> adj[v]; // Adjacency list

    // Adding edges to the graph (undirected)
    // adj[0].push_back(1);
    adj[0] = {1, 2};
    // adj[0].push_back(2);
    adj[1].push_back(0);
    adj[1].push_back(3);
    adj[1].push_back(4);
    adj[2].push_back(0);
    adj[3].push_back(1);
    adj[4].push_back(1);

    // Perform BFS
    std::vector<int> bfsResult = bfsOfGraph(v, adj);

    // Print BFS result
    std::cout << "BFS Traversal: ";
    for (int node : bfsResult) {
        std::cout << node << " ";
    }
    std::cout << std::endl;

    adj1[0] = {2,3};
    adj1[1] = {1,5,6};
    adj1[2] = {1,4,7};
    adj1[3] = {3,8};
    adj1[4] = {2};
    adj1[5] = {2};
    adj1[6] = {3,8};
    adj1[7] = {4,7};



    return 0;
}