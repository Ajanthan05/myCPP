#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <utility>
#include <algorithm>
#include <unordered_set>
#include <string>

using namespace std;

int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
    queue<pair<string, int>> q;
    q.push({beginWord, 1});
    unordered_set<string> st(wordList.begin(), wordList.end());
    st.erase(beginWord);

    while(!q.empty()) {
        string word = q.front().first;
        int steps = q.front().second;
        q.pop();
        if(word == endWord) return steps;

        // word_length * 26 * n * log(n){for set}
        for(int i=0; i<word.size(); i++) {
            char original = word[i];
            for(char ch='a'; ch<='z'; ch++) {
                word[i] = ch;
                // If exists ia the set
                if(st.find(word) != st.end()) {
                    st.erase(word);
                    q.push({word, steps + 1});
                }
            }
            word[i] = original;
        }
    }
    return 0;
}


vector<vector<string>> findLadders(string beginWord, string endWord, vector<string>& wordList) {
    unordered_set<string> st(wordList.begin(), wordList.end());
    queue<vector<string>> q;
    q.push({beginWord});
    vector<string> wordInLevel;
    wordInLevel.push_back(beginWord);
    vector<vector<string>> ans;
    int level = 0;

    while(!q.empty()) {
        vector<string> vec = q.front();
        q.pop();

        // erase all the words have been used in previous level
        if(vec.size() > level) {
            level++;
            for(auto it : wordInLevel) {
                st.erase(it);
            }
            wordInLevel.clear();
        }

        string word = vec.back(); 

        if (word == endWord) {
            // The first sequence where reach end
            if (ans.size() == 0) {
                ans.push_back(vec);
            }
            else if(ans[0].size() == vec.size()) {
                ans.push_back(vec);
            }
        }

        for(int i=0; i<word.size(); i++) {
            char original = word[i];
            for(char ch='a'; ch<='z'; ch++) {
                word[i] = ch;
                if(st.count(word)) {
                    vec.push_back(word);
                    q.push(vec);
                    // mark as visited on the level
                    wordInLevel.push_back(word);
                    // same level can have multiple answers 
                    vec.pop_back();
                }
            }
            word[i] = original;
        } 
    }
    return ans;
}

int main() {


    return 0;
}