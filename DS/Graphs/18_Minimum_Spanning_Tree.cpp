#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>

using namespace std;
class DisjointSet {
public:
    vector<int> rank, parent, size;

    DisjointSet(int n) {
        // +1 needed when 1 base indexing
        rank.resize(n+1, 0);
        parent.resize(n+1);

        size.resize(n+1);
        for(int i=0; i<=n; i++) {
            parent[i] = i;

            size[i] = 1;
        }
    }
    // Find Ultinate Parent
    int findUPar(int node) {    
        if (node == parent[node]) return node;
        // Path Compression:- Whenever return the value store it
        return parent[node] = findUPar(parent[node]);
    }

    void unionByRank(int u, int v) {
        int ulp_u = findUPar(u); // Ultimate parent of u
        int ulp_v = findUPar(v);

        // Check if they are belonging to same compnent
        if(ulp_u == ulp_v) return;

        // when smaller guy go and attach:- no change in rank
        if(rank[ulp_u] < rank[ulp_v]) {
            parent[ulp_u] = ulp_v;
        }
        else if(rank[ulp_v] < rank[ulp_u]) {
            parent[ulp_v] = ulp_u;
        }
        else {
            parent[ulp_v] = ulp_u;
            rank[ulp_u]++;
        }   
    }

    void unionBySize(int u, int v) {
        int ulp_u = findUPar(u); // Ultimate parent of u
        int ulp_v = findUPar(v);

        // Check if they are belonging to same compnent
        if(ulp_u == ulp_v) return;
        if(size[ulp_u] < size[ulp_v]) {
            parent[ulp_v] = ulp_u;
            size[ulp_u] += size[ulp_v];
        }
        else {
            parent[ulp_v] = ulp_u;
            size[ulp_u] += size[ulp_v];
        }
    }

    // bool isSame(DisjointSet dsp,int u, int v) {
    //     return (dsp.findUPar(u) == dsp.findUPar(v));
    // }
    bool isSame(int u, int v) {
        return findUPar(u) == findUPar(v);
    }
};

int spaningTree(int v, vector<vector<int>> adj[]) {
    /*  Bidirectional edge
    1 - 2 wt = 5
    1 -> (2,5)
    2 -> (1,5) no problem it automatically discade 2nd edge
    */
    vector<pair<int, pair<int, int>>> edges;
    for(int i=0; i<v; i++) {
        for(auto it : adj[i]) {
            int adjNode = it[0];
            int wt = it[1];
            int node = i;

            edges.push_back({wt, {node, adjNode}});
        } 
    }
    // std::range::sort(edges);
    sort(edges.begin(), edges.end());

    DisjointSet ds(v);
    int mstWt = 0;
    
    for (auto &[wt, uv] : edges) {
        auto &[u,v] = uv;

        if(ds.isSame(u, v)) {
            mstWt += 0;
            ds.unionByRank(u,v);
        }
    }
}


/* Number of Provinces */

int numProvience(vector<vector<int>> adj, int v) {
    DisjointSet ds(v);
    for(int i=0; i<v; i++) {
        for(int j=0; j<v; j++) {
            if(adj[i][j] == 1) {
                ds.unionBySize(i, j);
            }
        }
    }
    int cnt=0;
    for(int i=0; i<v; i++) {
        // if (ds.findUPar(i) == i) cnt++;
        // no need to do that
        if(ds.parent[i] == i) cnt++;
    }
    return cnt;
}

/* Number of Operations to Make Network Connected */
// error
// int makeConnected(int n, vector<vector<int>>& connections) {
//     DisjointSet ds(n);
//     int cntExtras = 0;
//     for(const auto &it : connections) {
//         int u = it[0];
//         int v = it[1];
//         if(ds.isSame(u, v)) cntExtras++;
//         else ds.unionBySize(u, v);
//     }
//     int cntconnectedComponent = 0;
//     for(int i=0; i<n; i++) // boc its zero base indexing 
//     {
//         if(ds.parent[i] == i) cntconnectedComponent++;
//     }
//     int ans = cntconnectedComponent - 1;
//     if (ans > cntExtras) return ans;
//     else -1;
// }
int main() {


    return 0;
}