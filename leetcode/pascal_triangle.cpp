class Solution {
public:
    typedef vector<int> Row;
    typedef vector<Row> Tri;
    Tri generate(int numRows) {
        // Start typing your C/C++ solution below
        // DO NOT write int main() function
        Tri result;
        
        for (int i = 0; i < numRows; ++i) {
            Row row(i+1, 1);
            
            // when result is empty, prev is no used.
            // just assume row to it to make compiler happy.
            Row &prev = result.size() > 0 ? result.back() : row;
            
            for (int j = 1; j < i; ++j) {
                row[j] = prev[j-1] + prev[j];
            }
            
            result.push_back(row);
        }
        
        return result;
    }
};
