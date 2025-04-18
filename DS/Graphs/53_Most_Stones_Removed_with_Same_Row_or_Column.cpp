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

int removeStones(vector<vector<int>>& stones) {
    int maxRow = 0;
    int maxCol = 0;
    for(auto it : stones) {
        maxRow = max(maxRow, it[0]);
        maxCol = max(maxCol, it[1]);
    }

    DisjointSet ds(maxRow + maxCol + 1);
    
    // WE treet row as nodes in Disjoint Set and columns as nodes in Disjoint Set
    // Treet rows as rows and colums as rows => colLwn + rowMaxLen + 1
    unordered_map<int, int> stoneNodes;
    for(auto it : stones) {
        int nodeRow = it[0];
        int nodeCol = it[1] + maxRow + 1;
        ds.unionBySize(nodeRow, nodeCol);
        stoneNodes[nodeRow] = 1;
        stoneNodes[nodeCol] = 1;
    }

    int cnt = 0;
    for(auto it : stoneNodes) {
        if(ds.findUPar(it.first) == it.first) cnt++;
    }

    return stones.size() - cnt;
}

int main() {


    return 0;
}