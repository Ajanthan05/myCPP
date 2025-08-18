#include <bits/stdc++.h>
using namespace std;

char trunRight(char c) {
    return (c == '9' ? '0' : c+1);
}

char turnLeft(char c) {
    return (c == '0' ? '9' : c-1);
}

vector<string> nextOperation(string s) {
    vector<string> ans;
    for(int i=0; i<4; ++i) {
        string copy = s;
        copy[i] = trunRight(s[i]);
        ans.push_back(copy);
        copy[i] = turnLeft(s[i]);
        ans.push_back(copy);
    }
    return ans;
}

int openLock(vector<string> &deadends, string target) {
    queue<string> q;
    unordered_map<string, bool> vis;
    q.push("0000");
    vis["0000"] = true;
    int level = 0;
    unordered_set<string> deadendSet(deadends.begin(), deadends.end());

    while(!q.empty()) {
        int sz = q.size();
        while (sz--)
        {
            string s = q.front();
            q.pop();

            if (s==target) return level;
            if (deadendSet.find(s) != deadendSet.end()) continue;

            for(string option : nextOperation(s)) {
                if(!vis[option]) {
                    q.push(option);
                    vis[option] = true;
                }
            }
        }
        level++;
    }
    return -1;
}





/*  Optimal */
void fillNeighbours(queue<string> &que,string &curr,unordered_set<string> &dead){
    for(int i = 0;i<4;i++){
        char ch = curr[i];

        char dec = ch == '0'?'9':ch-1;
        char inc = ch == '9'?'1':ch+1;

        curr[i] = dec;
        if(!dead.count(curr)){
            dead.insert(curr);
            que.push(curr);
        }

        curr[i] = inc;
        if(!dead.count(curr)){
            dead.insert(curr);
            que.push(curr);
        }
        curr[i]=ch;
    }
}
int openLock(vector<string>& deadends, string target) {
    unordered_set<string> dead(begin(deadends),end(deadends));

    string start = "0000";
    if(dead.count(start)) return -1;
    queue<string> que;
    que.push(start);

    int level = 0;
    while(!que.empty()){
        int n = que.size();

        while(n--){
            auto top = que.front();
            que.pop();

            if(top == target) return level;

            fillNeighbours(que,top,dead);
        }
        level++;
    }

    return -1;
}

int main() {

}

