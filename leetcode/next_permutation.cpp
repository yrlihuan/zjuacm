class Solution {
public:
    void nextPermutation(vector<int> &num) {
        int pos = num.size() - 2;
        while (pos >= 0) {
            int n0 = num[pos];
            int nn = num.back();
            
            if (n0 < nn) {
                int p = pos + 1;
                while (p < num.size()) {
                    if (n0 < num[p]) {
                        break;
                    }
                    ++p;
                }
                
                swap(num[pos], num[p]);
                break;
            }
            else {
                int p = pos;
                while (p < num.size() - 1) {
                    num[p] = num[p+1];
                    ++p;
                }
                
                num[p] = n0;
            
                pos--;
            } 
        }
        
        if (pos < 0) {
            sort(num.begin(), num.end());
        }
        
    }
};
