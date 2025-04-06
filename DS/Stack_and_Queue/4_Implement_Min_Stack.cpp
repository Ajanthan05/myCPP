#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>

using namespace std;

class MinStack {
private:
    stack<pair<int, int>> st;
public:
    MinStack() {
        
    }
    
    void push(int val) {
        if(st.empty()) {
            st.push({val, val});
        }
        else {
            st.push({val, min(val, st.top().second)});
        }
    }
    
    void pop() {
        st.pop();
    }
    
    int top() {
        return st.top().first;
    }
    
    int getMin() {
        return st.top().second;
    }
};
    
/* Optimize space */
class MinStack {
    private:
        stack<long long> st; // ✅ Use long long to avoid overflow
        long long minVal;
    
    public:
        MinStack() {
            minVal = INT_MAX;
        }
        
        void push(int val) {
            if (st.empty()) {
                minVal = val;
                st.push(val);
            } else {
                if (val < minVal) {
                    st.push(2LL * val - minVal); // ✅ Store encoded value
                    minVal = val;
                } else {
                    st.push(val);
                }
            }
        }
        
        void pop() {
            if (st.empty()) return;
    
            long long topVal = st.top();
            st.pop();
    
            if (topVal < minVal) { // ✅ Recover previous min
                minVal = 2 * minVal - topVal;
            }
        }
        
        int top() {
            if (st.empty()) return -1; // ✅ Return an indicator value
            
            long long topVal = st.top();
            return (topVal < minVal) ? minVal : topVal;
        }
        
        int getMin() {
            return (st.empty()) ? -1 : minVal; // ✅ Handle empty stack case
        }
    };

int main() {

    MinStack minStack;
    minStack.push(5);
    minStack.push(2);
    minStack.push(10);
    minStack.push(1);
    minStack.pop();
    cout << minStack.getMin() << endl; // Output: 2
    cout << minStack.top() << endl;    // Output: 10
    
    return 0;
}