/**
 * Definition for an interval.
 * struct Interval {
 *     int start;
 *     int end;
 *     Interval() : start(0), end(0) {}
 *     Interval(int s, int e) : start(s), end(e) {}
 * };
 */

bool operator< (const Interval &a, const Interval &b) {
    return a.start < b.start;
}
 
class Solution {
public:

    vector<Interval> insert(vector<Interval> &intervals, Interval newInterval) {
        // Start typing your C/C++ solution below
        // DO NOT write int main() function
        intervals.push_back(newInterval);
        
        sort(intervals.begin(), intervals.end());
        
        vector<Interval> result;
        Interval prev = intervals.front();
        vector<Interval>::iterator iter = intervals.begin();
        ++iter;
        while (iter < intervals.end()) {
            Interval &cur = *iter;
            if (cur.start <= prev.end) {
                prev.end = cur.end > prev.end ? cur.end : prev.end;
            }
            else {
                result.push_back(prev);
                prev = cur;
            }
            
            ++iter;
        }
        
        result.push_back(prev);
        
        return result;
    }
};
