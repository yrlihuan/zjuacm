class Solution {
public:
    int sqrt(int x) {
        // Start typing your C/C++ solution below
        // DO NOT write int main() function
        long long left = 0;
        long long right = x == 1 || x == 0 ? x + 1 : x;
        
        while (true) {
            if (right == left) {
                return left - 1;
            }
            
            long long mid = (right - left) / 2 + left;
            long long square = mid * mid;
            if (square == x) {
                return mid;
            }
            else if (square > x) {
                right = mid;
            }
            else {
                left = mid + 1;
            }
        }
    }
};
