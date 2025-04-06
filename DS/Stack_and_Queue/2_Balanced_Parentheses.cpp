#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>

using namespace std;

bool isValid(string s) {
    stack<char> st;
    // vector<int> opening = {}
    
    for(int i=0; i<s.size(); i++) {
        if (s[i] == '(' || s[i] == '[' || s[i] == '{') {
            st.push(s[i]);
        }
        else {
            if(st.empty()) return false;
            char ch = st.top();
            st.pop();
            if ( (s[i] == ')' && ch != '(')
                || (s[i] == ']' && ch != '[')
                || (s[i] == '}' && ch != '{')) {
                    return false;
                }
        }
    }
    return st.empty() ? true : false;
}

int main() {


    return 0;
}