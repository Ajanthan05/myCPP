#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>

using namespace std;

/* roads where roads[i] = [ui, vi, timei] */
// int countPaths(int n, vector<vector<int>>& roads) {
//     vector<pair<int, int>> adj[n];
//     for(auto it : roads) {
//         adj[it[0]].push_back({it[1], it[2]});
//         adj[it[1]].push_back({it[0], it[2]});
//     }
//     priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>> > pq;

//     vector<int> dist(n, 1e9), ways(n, 0);
//     dist[0] = 0;
//     ways[0] = 1;
//     pq.push({0,0});
//     int mod = (int)(1e9 + 7);
//     while(!pq.empty()) {
//         int dis = pq.top().first;
//         int node = pq.top().second;
//         pq.pop();

//         for(auto it : adj[node]) {
//             int adjNode = it.first;
//             int edW = it.second;
//             // This is the first time i am comming with the sortest distance
//             if (dis + edW < dist[adjNode]) {
//                 dist[adjNode] = dis + edW;
//                 pq.push({dis + edW, adjNode});
//                 ways[adjNode] = ways[node];
//             }
//             else if(dis + edW == dist[adjNode]) {
//                 ways[adjNode] = (ways[adjNode] +  ways[node]) % mod;
//             }
//         }
//     }
//     return ways[n-1] % mod;
// }

int countPaths(int n, vector<vector<int>>& roads) {
    vector<pair<int, long long>> adj[n];
    for(auto it : roads) {
        adj[it[0]].push_back({it[1], it[2]});
        adj[it[1]].push_back({it[0], it[2]});
    }
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>> > pq;

    vector<long long> dist(n, 1e18);
    vector<int> ways(n, 0);
    dist[0] = 0;
    ways[0] = 1;
    pq.push({0, 0});
    int mod = 1e9 + 7;
    
    while(!pq.empty()) {
        long long dis = pq.top().first;
        int node = pq.top().second;
        pq.pop();

        if (dis > dist[node]) continue;

        for(auto it : adj[node]) {
            int adjNode = it.first;
            long long edW = it.second;
            if (dis + edW < dist[adjNode]) {
                dist[adjNode] = dis + edW;
                pq.push({dist[adjNode], adjNode});
                ways[adjNode] = ways[node];
            }
            else if(dis + edW == dist[adjNode]) {
                ways[adjNode] = (ways[adjNode] +  ways[node]) % mod;
            }
        }
    }
    return ways[n-1];
}

/* Optimal */
const auto _ = std::cin.tie(nullptr)->sync_with_stdio(false);

// #define LC_HACK
// #ifdef LC_HACK
// const auto __ = []() {
//     struct ___ {
//         static void _() { std::ofstream("display_runtime.txt") << 0 << '\n'; }
//     };
//     std::atexit(&___::_);
//     return 0;
// }();
// #endif

class Solution {
public:
    int countPaths(int n, vector<vector<int>>& roads) {
        vector<vector<pair<int, int>>> adj(n);
        for(auto& road : roads) {
            int u = road[0], v = road[1], wt = road[2];
            adj[u].emplace_back(v, wt);
            adj[v].emplace_back(u, wt);
        }

        vector<long long> dist(n, LLONG_MAX);
        vector<int> ways(n, 0);
        priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;

        dist[0] = 0;
        ways[0] = 1;
        pq.push({0, 0});
        const int MOD = 1e9 + 7;

        while(!pq.empty()) {
            auto [d, u] = pq.top(); pq.pop();

            if (d > dist[u]) continue;

            for(auto& [v, wt] : adj[u]) {
                if (d + wt < dist[v]) {
                    dist[v] = d + wt;
                    ways[v] = ways[u];
                    pq.push({dist[v], v});
                } else if (d + wt == dist[v]) {
                    ways[v] = (ways[v] + ways[u]) % MOD;
                }
            }
        }

        return ways[n-1];
    }
};


int main() {


    return 0;
}