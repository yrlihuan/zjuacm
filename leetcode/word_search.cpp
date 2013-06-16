class Solution {
public:
    bool searchChar(string word, int pos, int x, int y, char *cells, bool *used, int rows, int cols) {
        if (pos == word.size()) {
            return true;
        }
        
        int adjs[4];
        adjs[0] = (x-1)*(cols+2) + y;
        adjs[1] = (x+1)*(cols+2) + y;
        adjs[2] = x*(cols+2) + y - 1;
        adjs[3] = x*(cols+2) + y + 1;
        
        for (int i = 0; i < 4; ++i) {
            int coord = adjs[i];
            if (used[coord]) {
                continue;
            }
            
            char c = cells[coord];
            if (c == word[pos]) {
                used[coord] = true;
                bool found = searchChar(word, pos+1, coord/(cols+2), coord%(cols+2), cells, used, rows, cols);
                used[coord] = false;
                
                if (found) {
                    return true;
                }
            }
        }
        
        return false;
    }

    bool exist(vector<vector<char> > &board, string word) {
        // Start typing your C/C++ solution below
        // DO NOT write int main() function
        
        if (board.size() == 0) {
            return false;
        }
        
        if (word.size() == 0) {
            return true;
        }
        
        int rows = board.size();
        int cols = board.front().size();
        
        char cells[(rows+2)*(cols+2)];
        bool used[(rows+2)*(cols+2)];
        
        memset(cells, '.', sizeof(cells));
        memset(used, 1, sizeof(used));
        
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                cells[(i+1)*(cols+2)+j+1] = board[i][j];
                used[(i+1)*(cols+2)+j+1] = false;
            }
            
        }
        
        char c0 = word[0];
        for (int x = 1; x <= rows; ++x) {
            for (int y = 1; y <= cols; ++y) {
                int ind = x * (cols+2) + y;
                if (cells[ind] == c0) {
                    used[ind] = true;
                    bool found = searchChar(word, 1, x, y, cells, used, rows, cols);
                    used[ind] = false;
                    if (found) {
                        return true;
                    }
                }
                
            }
        }
        
        return false;
    }
};
