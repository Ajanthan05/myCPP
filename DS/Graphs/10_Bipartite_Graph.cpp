#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>

using namespace std;

/* Bipartite Graph :- groph whthout cycle
    graph with even cycle length
*/

bool check(int n, vector<int> adj[], vector<int> &color, int start) {
    queue<int> q;
    q.push(start);
    color[start] = 0;

    while(!q.empty()) {
        int node = q.front();
        q.pop();

        for(auto it :adj[node]) {
            // If the adjacent node is yet not coloured
            // You will give the opposite color of the node
            if(color[it] == -1) {
                color[it] = !color[node];
                q.push(it);
            }
            // Is adjacent guy having the same color
            // Someone did color it on some other path
            else if(color[it] == color[node]) {
                return false;
            }
        }
    }
    return true;
}

bool isBipertite(int n, vector<int> adj[]) {
    vector<int> color(n, -1);

    for(int i=0; i<n; i++) color[i] = -1;

    for(int i=0; i<n; i++) {
        if(color[i] == -1) {
            if(check(n, adj, color, i) == false) {
                return false;
            }
        }
    }
    return true;
}

int main() {


    return 0;
}