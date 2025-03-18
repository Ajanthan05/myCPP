#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <utility>
#include <set>
#include <unordered_set>
#include <string>
#include <climits>
#include <algorithm>

using namespace std;

vector<int> dijkstra(vector<vector<pair<int, int>>> adj, int s) {
    int n = adj.size();
    priority_queue<pair<int ,int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    vector<int> dist(n , INT_MAX);
    dist[s] = 0;
    pq.push({0,5});

    while(!pq.empty()) {
        int dis = pq.top().first;
        int node = pq.top().second;
        pq.pop();

        for(auto it : adj[node]) {
            int edgeWeight = it.second;
            int adjNode = it.first;

            if (dis + edgeWeight < dist[adjNode]) {
                dist[adjNode] = dis + edgeWeight;
                pq.push({dist[adjNode], adjNode});
            }
        }
    }
    return dist;
}

vector<int> dijkstra_set(vector<vector<pair<int, int>>> adj, int s) {
    int n = adj.size();
    set<pair<int, int>> st; 
    vector<int> dist(n, 1e9);

    st.insert({0, s});
    dist[s] = 0;

    while(!st.empty()) {
        auto it = *(st.begin());
        int node = it.second;
        int dis = it.first;
        st.erase(it);

        for(auto it : adj[node]) {
            int adjNode = it.first;
            int edgW = it.second;

            if (dis + edgW < dist[adjNode]) {
                // Erase if it existed
                if(dist[adjNode] != 1e9) {
                    st.erase({dist[adjNode], adjNode});
                }
            }
        }
    }
    return dist;
}

// int shortestPathLength(vector<vector<int>>& graph) {
vector<int> shortestPath(int n, int m, vector<vector<int>> &edge) {
    vector<pair<int, int>> adj[n+1];
    for(auto it : edge) {
        adj[it[0]].push_back({it[1], it[2]});
        adj[it[1]].push_back({it[0], it[2]});
    }

    priority_queue<pair<int ,int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({0,1});

    vector<int> dist(n+1, 1e9);
    dist[1] = 0;

    vector<int> parent(n+1);
    for(int i=0; i<=n; i++) parent[i] = i;
    
    while(!pq.empty()) {
        auto it = pq.top();
        int node = it.second;
        int dis = it.first;
        pq.pop();

        for(auto it : adj[node]) {
            int adjNode = it.first;
            int edW = it.second;
            if( dis + edW < dist[adjNode]) {
                dist[adjNode] = dis + edW;
                pq.push({dis + edW, adjNode});
                parent[adjNode] = node;
            }
        }
    }
    if(dist[n] == 1e9) return {-1};
    
    vector<int> path;
    // Remember where I came from
    int node = n;
    while(parent[node] != node) {
        path.push_back(node);
        node = parent[node];
    }
    path.push_back(1);
    reverse(path.begin(), path.end());
    return path;
}

int main() {


    return 0;
}