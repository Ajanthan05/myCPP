#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>

using namespace std;

class DisjointSet {
    vector<int> rank, parent, size;

public:
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

int main() {

    DisjointSet ds(7);
    ds.unionByRank(1,2);
    ds.unionByRank(2,3);
    ds.unionByRank(4,5);
    ds.unionByRank(6,7);
    ds.unionByRank(5,6);

    cout << ds.isSame(3,7) << "\n";
    if (ds.findUPar(3) == ds.findUPar(7)) {
        cout<< "Same\n";
    }
    else cout<< "Not Same\n";

    ds.unionByRank(3,7);
    cout << ds.isSame(3,7) << "\n";

    {
    DisjointSet ds(7);
    ds.unionBySize(1,2);
    ds.unionBySize(2,3);
    ds.unionBySize(4,5);
    ds.unionBySize(6,7);
    ds.unionBySize(5,6);

    cout << ds.isSame(3,7) << "\n";
    if (ds.findUPar(3) == ds.findUPar(7)) {
        cout<< "Same\n";
    }
    else cout<< "Not Same\n";

    ds.unionBySize(3,7);
    cout << ds.isSame(3,7) << "\n";
    }
    return 0;
}