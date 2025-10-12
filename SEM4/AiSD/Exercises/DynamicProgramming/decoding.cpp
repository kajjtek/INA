#include <string>
#include <vector>

class Solution {
public:
    int numDecodings(std::string s) {
        std::vector<int> dp(s.size());
        if(correctOne(s[0])) dp[0]=1;
        dp[1]=correctTwo(s[0], s[1])?dp[0]+1:dp[0];

        for(int i=2; i<s.size(); i++){
            if(correctOne(s[i])) dp[i]+=dp[i-1];
            if(correctTwo(s[i-1],s[i])) dp[i]+=dp[i-2];
        }
        return dp[s.size()-1];
    }
    bool correctOne(char c){
        return c>='1' && c<='9';
    }
    bool correctTwo(char c1, char c2){
        return (c1>='1' && c1<='2') && (c2>='0' && c2<='6');
    }
};