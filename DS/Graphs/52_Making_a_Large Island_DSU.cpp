#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <set>
#include <unordered_map>
#include <unordered_set>

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

bool isValid(int newr, int newc, int n) {
    return (newr >= 0 && newr < n &&  newc >= 0 && newc < n);
}

int largestIsland(vector<vector<int>>& grid) {
    int n = grid.size();
    // step 1:
    DisjointSet ds(n * n);
    for(int row=0; row<n; row++) {
        for(int col=0; col<n; col++) {
            if (grid[row][col] == 0) continue;
            int dr[] = {-1,0,1,0};
            int dc[] = {0,-1,0,1};
            for(int ind=0; ind<4; ind++) {
                int newr = row + dr[ind];
                int newc = col + dc[ind];
                if (isValid(newr, newc, n) && grid[newr][newc]==1) {
                    int nodeNo = row * n + col;
                    int adjNodeNo = newr * n + newc;
                    ds.unionBySize(nodeNo, adjNodeNo);
                }
            }
        }
    }
    
    int mx = 0;
    for(int row=0; row<n; row++) {
        for(int col=0; col<n; col++) {
            if (grid[row][col] == 1) continue;
            int dr[] = {-1,0,1,0};
            int dc[] = {0,-1,0,1};
            set<int> components;
            for(int ind=0; ind<4; ind++) {
                int newr = row + dr[ind];
                int newc = col + dc[ind];
                if (isValid(newr, newc, n)) {
                    if(grid[newr][newc] == 1) {
                        components.insert(ds.findUPar(newr * n + newc));
                    }
                }
            }
            int sizeTotal = 0;
            for(auto it : components) {
                sizeTotal += ds.size[it];
            }
            sizeTotal++;
            mx = max(mx, sizeTotal);
        }
    }
    for(int cellNo=0; cellNo<n*n; cellNo++) {
        mx = max(mx, ds.size[ds.findUPar(cellNo)]);
    }
    return mx;
}


/* optimal */
class Solution {
public:

    static constexpr int X[4] = {0, 0, 1, -1};
    static constexpr int Y[4] = {1, -1, 0, 0};

    int dfs(vector<vector<int>>& grid, int x, int y, int &index){
        int ans = 1;
        grid[x][y] = index;
        for(int i = 0; i < 4; i++){
            int xx = x + X[i];
            int yy = y + Y[i];
            if(xx >= 0 && xx < grid.size() && yy >= 0 && yy < grid[0].size() && grid[xx][yy] == 1){
                ans += dfs(grid, xx, yy, index);
            }
        }
        return ans;
    }

    int largestIsland(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();
        unordered_map<int, int> mp;
        int index = 2;
        for(int i = 0; i < m; i++){
            for(int j = 0; j < n; j++){
                if(grid[i][j] == 1){
                    mp[index] = dfs(grid, i, j, index);
                    index++;
                }
            }
        }
        if(mp.size() == 0) return 1;
        if(mp.size() == 1){
            if(mp[index-1] < m*n) return mp[index-1]+1;
            else return mp[index-1];
        }
        int ans = 0;
        for(int i = 0; i < m; i++){
            for(int j = 0; j < n; j++){
                if(grid[i][j] == 0){
                    unordered_set<int> s;
                    if(i-1 >= 0 && grid[i-1][j] != 0) s.insert(grid[i-1][j]);
                    if(i+1 < m && grid[i+1][j] != 0) s.insert(grid[i+1][j]);
                    if(j-1 >= 0 && grid[i][j-1] != 0) s.insert(grid[i][j-1]);
                    if(j+1 < n && grid[i][j+1] != 0) s.insert(grid[i][j+1]);
                    int sum = 1;
                    for(int k : s){
                        sum += mp[k];
                    }
                    ans = max(ans, sum);
                }
            }
        }
        return ans;
    }
};

int main() {

    
    return 0;
}