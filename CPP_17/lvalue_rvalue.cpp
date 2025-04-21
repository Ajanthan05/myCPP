#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <utility>
#include <algorithm>
#include <stack>

using namespace std;

// normaly function return an rvalue
// So that we can't assign fun() = 5; // error expression must be a modifiable lvalue

// we are returning lvalue reference
int& fun() {
    static int s = 5;
    return s;
}

// U cannot take lvalue reference from rvalue
void SetValue(int &v) {

}

void PrintName(const string& name) {}

void RValue_Reference(const string&& name) {}


// 3 Find lvalue or rvalue
void lvalueORrvalue(const string& name) {cout << "lvalue\n";}
void lvalueORrvalue(string&& name) {cout << "rvalue\n";} //  even thow const & can  -> If it exist it will be chosen
// take rvlue it go for 2nd one

// rvalues are temperory. so we can steal it and use it some where 
// lvalues are basically some kind of storage back in them, rvalues are temporary values
int main() {

    int i = fun();
    fun() = 6;

    int x = 10;
    SetValue(i); // lvalue
    // SetValue(10); // rvalue throw error when we put &
    // It won't throw error when "const int &v"


    const int& t = 10; // compiler create a temperory memory and taking it reference
   
    string s1 = "Ajan";
    string s2 = "Thaar";

    string name = s2 + s1; // here s1+s2 create temp object rvalue, 
    // then it assign to lvalue  

    PrintName(name);
    PrintName(s1 + s2); // Without constant it wont work boc it's an rvalue

    // RValue Reference:- function that only accepts temperary object

    // RValue_Reference(name); error an rvalue reference cannot be bound to an lvalue
    RValue_Reference(s1 + s2);
    return 0;
}