class Solution {
public:
    typedef vector<int> VI;
    
    vector<VI> subsets(VI &S) {
        // Start typing your C/C++ solution below
        // DO NOT write int main() function
        
        vector<VI> result;
        if (S.size() == 0) {
            result.push_back(VI());
            return result;
        }
        
        int first = S.front();
        VI child(S.begin() + 1, S.end());
        vector<VI> subsetsChild = subsets(child);
        
        vector<VI>::iterator iter = subsetsChild.begin();
        while (iter < subsetsChild.end()) {
            VI &set = *iter;
            sort(set.begin(), set.end());
            result.push_back(set);
            
            set.push_back(first);
            sort(set.begin(), set.end());
            result.push_back(set);
            
            iter++;
        }
        
        return result;
    }
};
