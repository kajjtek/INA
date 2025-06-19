#include <vector>
class Solution {
public:
    int trap(std::vector<int>& height) {
        std::vector<int> dp(height.size(), 0);
        for(int i=0; i<height.size(); i++){
            int j=i+1;
            int val=0;
            while(height[j]<height[i] && j<=height.size()){
                val++;
                if(j==height.size()-1) val=0;
                j++;
            }
            int previous;
            if(i>0) previous=dp[i-1];
            else previous = 0;
            dp[i]=previous+val;
        }
        return dp[height.size()-1];
    }
};

int main(){
    std::vector<int> height = {0,1,0,2,1,0,1,3,2,1,2,1};
    Solution sol;
    int result = sol.trap(height);
}