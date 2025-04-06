#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <utility>
#include <algorithm>

using namespace std;

class Q_Arr {
private:
    int n = 4;
    int start = -1;
    int end = -1;
    int arr[n];
    int curSize = 0;

public:
    void push(int el) {
        if(curSize == n) {
            return;
        }
        if(curSize == o) {
            start = 0;
            end = 0;
        }
        else {
            end = (end + 1) % n;
        }
        arr[end] = el;
        curSize++;
    }
    
    int pop() {
        if(curSize == 0) {
            cout << "ISEmpty\n";
        }
        int el = arr[start];
        if(curSize == 1) {
            start = end = -1;
        }
        else {
            start = (start + 1) % n;
        }
        curSize--;
        return el;
    }
    int top() {
        if(curSize == 0) {
            cout << "ISEmpty\n";
            return ;
        }
        return arr[start];
    }

    int size() {
        return curSize;
    }
};


struct Node {
    int val;
    Node* next;

    explicit Node(int val=0, Node *next=nullptr) : val(val), next(next) {}
};

class StackLL {
private:
    Node *top;
    int curSize;

public:
    StackLL() {
        top = nullptr;
        curSize = 0;
    }

    void push(int el) {
        if(!top) {
            Node *tmp = new Node(el, top);
            top = tmp;
            return;
        }
        curSize++;
    }
    
    void pop() {
        Node *tmp = top;
        if(!tmp) {
            cout << "ISEmpty\n";
            return;
        }
        top = top->next;
        delete tmp;
        curSize--;
        return;
    }

    int top() {
        if(!top) {
            cout << "ISEmpty\n";
            return;
        }
        return top->val;
    }

    int size() {
        return curSize;
    }
};

class QueueLL {
private:
    int curSize = 0;
    Node* start;
    Node* end;

public:
    QueueLL() {
        int curSize = 0;
        start = nullptr;
        end = nullptr;
    }

    void push(int el) {
        Node *tmp = new Node(el, top);
        if(!start) {
            start = tmp;
            end = tmp;
        }
        else {
            end->next = tmp;
        }
        curSize++;
    }
    
    void pop() {
        if(!start) {
            cout << "ISEmpty\n";
            return;
        }
        Node *tmp = start;
        start = start->next;
        delete tmp;
        curSize--;
        return;
    }

    int top() {
        if(!start) {
            cout << "ISEmpty\n";
            return;
        }
        return start->val;
    }

    int size() {
        return curSize;
    }
};

class StackUsingQ {
private:
    queue<int> q;

public:
    void push(int el) {
        int n = q.size();
        q.push(el);
        for(int i=0; i<n-1; i++) {
            q.push(q.front());
            q.pop();
        }
    }
    
    void pop() {
        q.pop();
    }

    int top() {
        return q.front();
    }

    int size() {
        return q.size();
    }
};

class StackUsingQ {
private:
    stack<int> s1,s2;

public:
    void push(int el) {
        int n = s1.size();
        while(n) {
            s2.push(s1.top());
            s1.pop();
        }
        s1.push(el);
        while(n) {
            s1.push(s2.top());
            s2.pop();
        }
    }
    
    void pop() {
        s1.pop();
    }

    int top() {
        return s2.top();
    }

    int size() {
        return s1.size();
    }
};

/* OPTIMIZE PUSH*/
class StackUsingQ2 {
private:
    stack<int> s1,s2;

public:
    void push(int el) {
        s1.push(el);
    }
    
    void pop() {
        if(!s2.empty()) {
            s2.pop();
        }
        else {
            while(s1.size()) {
                s2.push(s1.top());
                s1.pop();
            }
            s2.pop();
        }
    }

    int top() {
        if(!s2.empty()) {
            return s2.top();
        }
        else {
            while(s1.size()) {
                s2.push(s1.top());
                s1.pop();
            }
            return s2.top();
        }
    }

    int size() {
        return s1.size();
    }
};

int main() {


    return 0;
}