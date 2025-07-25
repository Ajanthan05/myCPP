#include <iostream>
#include <vector>
#include <climits>
#include <string>
#include <set>


int lengthOfLongestSubstring(std::string s) {
    int maxLen = 0;
    int n = s.length();  

    for (int i=0; i<n; i++) {
        int hash[256] = {0};    // ASCII character set

        for (int j=i; j<n; j++) {
            if (hash[s[j]] == 1) break;
            // len = ;
            maxLen = std::max(j-i+1, maxLen);
            hash[s[j]] = 1;
        }
    }
    return maxLen;
}

// Longest Substring Without Repeating Characters 
int lengthOfLongestSubstring_SW(std::string s) {
    int maxLen = 0;
    int n = s.length();  
    // std::pair<int, int> hash[256];
    int hash[256];
    std::fill(hash, hash + 256, -1); // Initialize the array properly
    int l=0, r=0;

    while (r < n) {  // no equal
        if(hash[s[r]] != -1) {
            if(hash[s[r]] >= l) {
                l = hash[s[r]] + 1;
            }
        }
        int len = r - l + 1;
        maxLen = std::max(len, maxLen);
        hash[s[r]] = r;
        r++;
    }
    return maxLen;
}

/* 1004. Max Consecutive Ones III

*/
int longestOnes(std::vector<int>& nums, int k) {
    int n = nums.size();
    int l=0, r=0;
    int maxlen = 0;
    int numZeros = 0;

    while (r < n) {
        if(nums[r] == 0) numZeros++;
        while(numZeros > k) {
            if(nums[l] == 0) numZeros--;
            l++; 
        }
        
        if (numZeros <= k) maxlen = std::max(r - l + 1, maxlen);
        r++;
    }
    return maxlen;
}

int longestOnes(std::vector<int>& nums, int k) {
    int n = nums.size();
    int l=0, r=0;
    int maxlen = 0;
    int numZeros = 0;

    while (r < n) {
        if(nums[r] == 0) numZeros++;
        if (numZeros > k) {
            if(nums[l] == 0) numZeros--;
            l++; 
        }
        
        if (numZeros <= k) maxlen = std::max(r - l + 1, maxlen);
        r++;
    }
    return maxlen;
}



int totalFruit(std::vector<int>& fruits) {
    int n = fruits.size();
    int maxlen = 0;

    for(int i=0; i<n; i++) {
        std::set<int> take;
        for (int j=i; j<n; j++) {
            take.insert(fruits[j]);
            if(take.size() <= 2) {
                maxlen = std::max(j - i +1, maxlen);
            }
            else {
                break;
            }
        }
    }
    return maxlen;
}

int totalFruit(std::vector<int>& fruits) {
    
}


/* NOTE
ASCII has 128 standard characters (0-127), but extended ASCII (e.g., Latin-1) has up to 256 characters (0-255).
'A' → 65
'Z' → 90
*/

int main() {

    std::string s = "cadbzabcd";
    std::cout << "Length of longest substring: " << lengthOfLongestSubstring(s) << std::endl;

    std::vector<int> nums = {1,1,1,0,0,0,1,1,1,1,0};
    std::cout << "longestOnes: " << longestOnes(nums, 2) << "\n";


    char c = 'G'; 
  
    std::cout << c << " in lowercase is represented as = "; 
  
    // tolower() returns an int value there for typecasting 
    // with char is required 
    std::cout << (char)tolower(c) << "\n";

    // string to be converted to lowercase 
    std::string str = "GEEKSFORGEEKS"; 
    
    for (auto& x : str) { 
        x = tolower(x); 
    } 
    
    std::cout << str << "\n"; 

    std::string str2 ="Geeks@123";
    for (auto& x : str2) { 
        x = tolower(x); 
    } 
    
    std::cout << str2 << "\n"; 
/*
lowercase character

special symbol

digit


tolower() will return the character as it is.
*/

    // Convert to upper case
    char ch = 'b';
    // char upperCh = ch - 32;  // Convert to uppercase
    char upperCh;
    if (ch >= 'a' && ch <= 'z') {
        upperCh = ch - 32;
    }
    std::cout << "Uppercase: " << upperCh << std::endl;  // Output: B

    char ch2 = 'b';
    char upperCh2 = std::toupper(ch2);
    std::cout << "Uppercase: " << upperCh2 << std::endl;  // Output: B
    
    return 0;
}