#include <iostream>
#include <stack>
#include <string>

using namespace std;

int priority(char ch) {
    if (ch == '^') return 3;
    else if (ch == '*' || ch == '/') return 2;
    else if (ch == '+' || ch == '-') return 1;
    return 0;
}

string infixToPostfix(string s) {
    int n = s.size();
    stack<char> st; // ✅ Fix: Use char stack
    string ans;

    for (int i = 0; i < n; i++) {
        if (isalnum(s[i])) { // ✅ Fix: Check if letter or digit
            ans += s[i];
        } 
        else if (s[i] == '(') {
            st.push(s[i]);
        } 
        else if (s[i] == ')') {
            while (!st.empty() && st.top() != '(') {
                ans += st.top();
                st.pop(); // ✅ Fix: Properly pop the operator
            }
            if (!st.empty()) st.pop(); // ✅ Remove '(' from stack
        } 
        else { // Operator handling
            while (!st.empty() && priority(s[i]) <= priority(st.top())) {
                ans += st.top();
                st.pop();
            }
            st.push(s[i]); // ✅ Push current operator
        }
    }
    
    // Pop remaining operators
    while (!st.empty()) {
        ans += st.top();
        st.pop();
    }

    return ans;
}


//////////////////////////
string PostfixToInfix(string s) {
    stack<string> st;

    for (char ch : s) {
        if (isalnum(ch)) { // ✅ Check if letter or digit
            st.push(string(1, ch)); // ✅ Convert char to string properly
        } 
        else { // Operator
            if (st.size() < 2) return "Invalid Expression"; // ✅ Stack underflow check

            string t1 = st.top();
            st.pop();
            string t2 = st.top();
            st.pop();

            string con = "(" + t2 + ch + t1 + ")";
            st.push(con);
        }
    }

    return (st.size() == 1) ? st.top() : "Invalid Expression"; // ✅ Final check
}

// Example usage
/*
TC = O(N){for while loop} + O(N1 + N2){Adition of string here N = N1 + N2}
*/
int main() {
    string expr = "a+b*(c^d-e)^(f+g*h)-i";
    cout << "Postfix: " << infixToPostfix(expr) << endl;

    string postfix = "ab+c*d-";
    cout << "Infix: " << PostfixToInfix(postfix) << endl;

    return 0;
}
