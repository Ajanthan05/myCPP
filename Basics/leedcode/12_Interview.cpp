#include <iostream>
#include <vector>
#include <map>

// std::vector<int> 
void order(std::vector<int> vec) {
    std::map<int, int> reverseMap;
    int ind = -1;

    for(int i=0; i<vec.size(); i++) {
        if(vec[i] == -1) ind = i;
        reverseMap[vec[i]] = i;
    }

    while(ind != NULL) {
        std::cout << ind << " ";
        ind = reverseMap[ind];
    }
}

int main() {

    std::vector<int> vec = {-1,4,0,1,2,3};
    order(vec);

    return 0;
}