class Solution {
public:
    typedef vector<int> PermType;
    typedef vector<PermType> PermArrayType;
    
    PermArrayType permRecursive(PermType &num) {
        int size = num.size();
        if (size == 1) {
            return PermArrayType(1, num);
        }
        
        set<int> firstElements;
        PermArrayType result;
        
        for (int i = 0; i < num.size(); ++i) {
            int firstCandidate = num[i];
            if (firstElements.find(firstCandidate) != firstElements.end()) {
                continue;
            }
            
            firstElements.insert(num[i]);
            
            PermType subsequence(size-1);
            copy(num.begin(), num.begin()+i, subsequence.begin());
            copy(num.begin()+i+1, num.end(), subsequence.begin()+i);
            
            PermArrayType subperms = permRecursive(subsequence);
            PermArrayType::iterator it = subperms.begin();
            while (it != subperms.end()) {
                result.push_back(PermType(size));
                PermType &p = result.back();
                
                p[0] = num[i];
                copy(it->begin(), it->end(), p.begin()+1);
                
                ++it;
            }
        }
        
        return result;
    }

    vector<vector<int> > permuteUnique(vector<int> &num) {
        // Start typing your C/C++ solution below
        // DO NOT write int main() function
        return permRecursive(num);
    }
};
