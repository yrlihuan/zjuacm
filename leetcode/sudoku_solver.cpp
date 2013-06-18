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
    
    bool solveRecursive(int i,
                        int j,
                        vector<vector<char> > &result,
                        int rowFill[],
                        int colFill[],
                        int blockFill[])
    {
        if (i == 9) {
            return true;
        }
        
        int next_j = (j + 1) % 9;
        int next_i = next_j == 0 ? i + 1 : i;
        
        if (result[i][j] != '.') {
            return solveRecursive(next_i, next_j, result, rowFill, colFill, blockFill);
        }
        
        int block = i/3*3+j/3;
        for (char num = '1'; num <= '9'; num++) {
            int mask = charToMask(num);
            
            if ((mask & rowFill[i]) || (mask & colFill[j]) || (mask & blockFill[block])) {
                continue;
            }
            
            result[i][j] = num;
            rowFill[i] |= mask;
            colFill[j] |= mask;
            blockFill[block] |= mask;
            
            bool matched = solveRecursive(next_i, next_j, result, rowFill, colFill, blockFill);
            if (matched) {
                return true;
            }
            
            result[i][j] = '.';
            rowFill[i] &= ~mask;
            colFill[j] &= ~mask;
            blockFill[block] &= ~mask;
        }
    }

    void solveSudoku(vector<vector<char> > &result) {
        int rowFill[9] = {0};
        int colFill[9] = {0};
        int blockFill[9] = {0};
        
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                char c = result[i][j];
                if (c == '.') {
                    continue;
                }
                else {
                    int mask = charToMask(c);
                    
                    rowFill[i] |= mask;
                    colFill[j] |= mask;
                    blockFill[i/3*3+j/3] |= mask;
                }
            }
        }
        
        bool matched = solveRecursive(0, 0, result, rowFill, colFill, blockFill);
    }
};
