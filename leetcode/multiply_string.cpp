class Number {
public:
    
    Number() {
        parts.push_back(0);
        shift = 0;
    }

    Number(string val) {
        shift = 0;
        
        int end = val.size();
        
        while (end > 0) {
            if (end > 4) {
                string sub = val.substr(end-4, 4);
                parts.push_back(atoi(sub.c_str()));
            }
            else {
                string sub = val.substr(0, end);
                parts.push_back(atoi(sub.c_str()));
            }
                
            end -= 4;
        }
    }
    
    Number(int val) {
        shift = 0;
        
        if (val == 0) {
            parts.push_back(0);
            return;
        }
        
        while (val) {
            int p = val % 10000;
            parts.push_back(p);
            
            val /= 10000;
        }
    }
    
    string to_str() {
        string result = "";
        for (int i = 0; i < parts.size() - 1; ++i) {
            char buf[8];
            sprintf(buf, "%04d", parts[i]);
            
            result = string(buf) + result;
        }
        
        char buf[8];
        sprintf(buf, "%d", parts.back());
        result = string(buf) + result;
        int pos = 0;
        while (pos++ < shift) {
            result += "0000";
        }
        
        return result;
    }
    
    vector<int> parts;
    int shift;
};

Number operator+ (const Number &a, const Number &b) {
    Number result;
    vector<int> &parts = result.parts;
    parts.pop_back();
    int &shift = result.shift;
    
    shift = a.shift < b.shift ? a.shift : b.shift;
    
    int carry = 0;
    int orders = shift;
    while (true) {
        
        int pa = orders - a.shift;
        int pb = orders - b.shift;
        
        int va, vb;
        if (pa >= 0 && pa < a.parts.size()) {
            va = a.parts[pa];
        }
        else {
            va = 0;
        }
        
        if (pb >= 0 && pb < b.parts.size()) {
            vb = b.parts[pb];
        }
        else {
            vb = 0;
        }
        
        int vc = va + vb + carry;
        if (vc >= 10000) {
            vc -= 10000;
            carry = 1;
        }
        else {
            carry = 0;
        }
        
        if (vc > 0 || carry > 0 || pa < a.parts.size() || pb < b.parts.size()) {
            parts.push_back(vc);
        }
        else {
            break;
        }
        
        orders += 1;
    }
    
    return result;
}

Number operator* (const Number &a, const Number &b) {
    Number result;
    
    for (int i = 0; i < a.parts.size(); ++i) {
        for (int j = 0; j < b.parts.size(); ++j) {
            int va = a.parts[i];
            int vb = b.parts[j];
                
            int mul = va * vb;
            Number num(mul);
            num.shift = a.shift + b.shift + i + j;
            
            result = result + num;
        }
    }
    
    while (result.parts.size() > 1 && result.parts.back() == 0) {
        result.parts.pop_back();
    }
    
    return result;
}

class Solution {
public:
    string multiply(string num1, string num2) {
        // Start typing your C/C++ solution below
        // DO NOT write int main() function
        Number n1(num1);
        Number n2(num2);
        
        Number n3 = n1 * n2;
        
        char buf[2] = "0";
        buf[0] += n3.parts.size();
        // return string(buf);
        return string(n3.to_str());
    }
};

