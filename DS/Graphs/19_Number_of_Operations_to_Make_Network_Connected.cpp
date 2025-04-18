#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>

using namespace std;

class DisjointSet {
    vector<int> parent, size;

public:
    DisjointSet(int n) {
        parent.resize(n);
        size.resize(n, 1);
        for (int i = 0; i < n; ++i) parent[i] = i;
    }

    int findUPar(int node) {
        if (parent[node] == node) return node;
        return parent[node] = findUPar(parent[node]);
    }

    void unionBySize(int u, int v) {
        int pu = findUPar(u);
        int pv = findUPar(v);
        if (pu == pv) return;
        if (size[pu] < size[pv]) {
            parent[pu] = pv;
            size[pv] += size[pu];
        } else {
            parent[pv] = pu;
            size[pu] += size[pv];
        }
    }

    bool isSame(int u, int v) {
        return findUPar(u) == findUPar(v);
    }

    int countComponents() {
        int cnt = 0;
        for (int i = 0; i < parent.size(); i++) {
            if (parent[i] == i) cnt++;
        }
        return cnt;
    }
};

class Solution {
public:
    int makeConnected(int n, vector<vector<int>>& connections) {
        if (connections.size() < n - 1) return -1; // Not enough cables

        DisjointSet ds(n);
        int cntExtras = 0;

        for (const auto& conn : connections) {
            int u = conn[0], v = conn[1];
            if (ds.isSame(u, v)) cntExtras++;
            else ds.unionBySize(u, v);
        }

        int components = ds.countComponents();
        int neededCables = components - 1;

        return (cntExtras >= neededCables) ? neededCables : -1;
    }
};

    
/* Optimal */

class DisjointSet_O{
    vector<int>rank,parent;
public:
    DisjointSet_O(int n)
    {
        rank.resize(n);
        parent.resize(n);
        for(int i = 0; i < n; i++)
            parent[i] = i;
    }
    int findParent(int node)
    {
        if(node == parent[node])
            return parent[node];
        return parent[node] = findParent(parent[node]);
    }

    void unionSet(int n1, int n2)
    {
        n1 = findParent(n1);
        n2 = findParent(n2);
        if(rank[n1] == rank[n2])
        {
            rank[n1]++;
            parent[n2] = n1;
        }
        else if(rank[n1]>rank[n2])
            parent[n2] = n1;
        else
            parent[n1] = n2;
    }

};


class Solution2 {
public:
    int makeConnected(int n, vector<vector<int>>& c) {
        int m = (int)c.size();
        if(m < n-1)
            return -1;
        DisjointSet_O obj(n);
        for(auto &it : c)
            obj.unionSet(it[0],it[1]);
        int cnt = 0;
        for(int i = 0; i< n; i++)
            if(obj.findParent(i)==i)
                cnt++;
        return cnt-1;
    }
};


int main() {


    return 0;
}