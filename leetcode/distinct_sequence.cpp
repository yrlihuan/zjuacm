class Solution {
public:
    int solveRecursive(const string &s, const string &t, int *mat[], int m, int n) {
        if (mat[m][n] != -1) {
            return mat[m][n];
        }
        
        int c = solveRecursive(s, t, mat, m-1, n);
        if (s[m-1] == t[n-1]) {
            c += solveRecursive(s, t, mat, m-1, n-1);
        }
        
        mat[m][n] = c;
        
        return c;
    }

    int numDistinct(string S, string T) {
        // Start typing your C/C++ solution below
        // DO NOT write int main() function
        
        // build the result matrix
        int sizeS = S.size();
        int sizeT = T.size();
        int *mat[sizeS+1];
        for (int i = 0; i <= sizeS; ++i) {
            mat[i] = new int[sizeT+1];
            for (int j = 0; j <= sizeT; ++j) {
              if (j == 0) {
                mat[i][j] = 1;
              }
              else if (i == 0) {
                mat[i][j] = 0;
              }
              else {
                mat[i][j] = -1;
              }
            }
        }
        
        int result = solveRecursive(S, T, mat, S.size(), T.size()); 
        
        for (int i = 0; i < sizeS; ++i) {
            delete[] mat[i];
        }
        
        return result;
    }
};
