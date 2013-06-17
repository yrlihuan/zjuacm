class Solution {
public:
    bool isValid(string s) {
        // Start typing your C/C++ solution below
        // DO NOT write int main() function
        
        string opens = "([{";
        string closes = ")]}";
        
        
        vector<int> stack;
        int pos = 0;
        while (pos < s.size()) {
            char c = s[pos];
            
            int ind = opens.find(c);
            if (ind != string::npos) {
                stack.push_back(ind);
            }
            else {
                if (stack.size() == 0) {
                    return false;
                }
                
                ind = closes.find(c);
                if (stack.back() == ind) {
                    stack.pop_back();
                }
                else {
                    return false;
                }
            }
            
            ++pos;
        }
        
        return stack.size() == 0;
        
    }
};
