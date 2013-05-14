class Solution {
public:
    typedef vector<int> PermType;
    vector<vector<int> > permuteRecursive(PermType &num) {
        if (num.size() == 1) {
            return vector<PermType>(1, num);
        }
        
        vector<PermType> result;
        int size = num.size();
        for (int i = 0; i < size; ++i) {
            PermType perm(size-1);
            copy(num.begin(), num.begin()+i, perm.begin());
            copy(num.begin()+i+1, num.end(), perm.begin()+i);
            
            vector<PermType> subperms = permuteRecursive(perm);
            
            vector<PermType>::iterator it = subperms.begin();
            while (it != subperms.end()) {
                PermType p(size);
                p[0] = num[i];
                copy(it->begin(), it->end(), p.begin()+1);
                
                result.push_back(p);
                ++it;
            }
        }
        
        return result;
    }

    vector<vector<int> > permute(vector<int> &num) {
        return permuteRecursive(num);
    }
};
