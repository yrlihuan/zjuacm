class Solution {
public:
    typedef vector<int> Row;
    typedef vector<Row> Tri;
    
    Row getRow(int rowIndex) {
        // Start typing your C/C++ solution below
        // DO NOT write int main() function
        
        if (rowIndex < 2) {
            return Row(rowIndex+1, 1);
        }
        
        int ind = 2;
        Row prev = Row(2, 1);
        
        while (ind <= rowIndex) {
            Row row = Row(ind+1, 1);
            
            for (int j = 1; j < ind; ++j) {
                row[j] = prev[j-1] + prev[j];
            }
            
            if (ind == rowIndex) {
                return row;
            }
            
            prev = row;
            ++ind;
        }
    }
};
