class Solution {
public:
    

    int numTrees(int n) {
        // use dynamic programing to solve the problem
        long long *numTreesSizeN = new long long[n>1?n+1:2];
        
        numTreesSizeN[0] = 1;
        numTreesSizeN[1] = 1;
        
        for (int i = 2; i <= n; ++i) {
            long long cnt = 0;
            for (int j = 0; j < i; ++j) {
                cnt += numTreesSizeN[j] * numTreesSizeN[i-j-1];
            }
            
            numTreesSizeN[i] = cnt;
        }
        
        long long result = numTreesSizeN[n];
        
        delete []numTreesSizeN;
        
        return result;
    }
};
