#include <iostream>
#include <string>
#include <vector>
using namespace std;

bool isIsomorphic(string s, string t) {
    if (s.size() != t.size()) return false;

    vector<int> mapS(256, -1);  // ASCII mapping for s → t
    vector<int> mapT(256, -1);  // ASCII mapping for t → s

    for (int i = 0; i < s.size(); ++i) {
        char cs = s[i], ct = t[i];

        if (mapS[cs] == -1 && mapT[ct] == -1) {
            mapS[cs] = ct;
            mapT[ct] = cs;
        } else {
            if (mapS[cs] != ct || mapT[ct] != cs)
                return false;
        }
    }

    return true;
}
