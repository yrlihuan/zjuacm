class Solution {
public:
    int charToMask(char c) {
        int p = c - '0';
        int mask = 1;
        
        while (--p) {
            mask <<= 1;
        }
        
        return mask;
    }
    
    bool isValidSudoku(vector<vector<char> > &result) {
        int rowFill[9] = {0};
        int colFill[9] = {0};
        int blockFill[9] = {0};
        
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                char c = result[i][j];
                int block = i/3*3+j/3;
                if (c == '.') {
                    continue;
                }
                else {
                    int mask = charToMask(c);
                    
                    if ((mask & rowFill[i]) || (mask & colFill[j]) || (mask & blockFill[block])) {
                        return false;
                    }
                    
                    rowFill[i] |= mask;
                    colFill[j] |= mask;
                    blockFill[block] |= mask;
                }
            }
        }
        
        return true;
    }
};
