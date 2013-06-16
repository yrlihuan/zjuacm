class Solution {
public:
    int divide(int dividend, int divisor) {
        if (dividend == 0) {
            return 0;
        }
        
        int sign = dividend >= 0 ? 1 : -1;
        sign = divisor > 0 ? sign : -sign;
        
        unsigned long long a = llabs(divisor);
        unsigned long long b = llabs(dividend);
        
        unsigned long long result;
        unsigned long long bit = 0x1;
        unsigned long long shifted = a;
        
        while (shifted <= b) {
            shifted <<= 1;
            bit <<= 1;
        }
        
        while (bit > 0) {
            shifted >>= 1;
            bit >>= 1;
            
            if (shifted <= b) {
                b -= shifted;
                result |= bit;
            }
        }
        
        if (sign == 1) {
            return result;
        }
        else {
            return -result;
        }
        
    }
};
