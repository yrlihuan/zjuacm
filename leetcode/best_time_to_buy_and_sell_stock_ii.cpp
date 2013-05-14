//Say you have an array for which the ith element is the price of a given stock on day i.
//
//Design an algorithm to find the maximum profit. You may complete as many transactions as you like (ie, buy one and sell one share of the stock multiple times). However, you may not engage in multiple transactions at the same time (ie, you must sell the stock before you buy again).
class Solution {
    typedef vector<int> ListType;
public:
    int maxProfit(vector<int> &prices) {
        // the trick here is:
        // when the price is lower than prev one, switch the buy price
        // to the lower.
        // if the price is highter than prev one, lock the profit by selling
        
        // try to solve this in one iteration
        
        ListType::iterator it = prices.begin();
        ListType::iterator end = prices.end();
        
        int buyPrice = numeric_limits<int>::max();
        int profit = 0;
        
        while (it < end) {
            int price = *it;
            if (price < buyPrice) {
                buyPrice = price;
            }
            else {
                profit += price - buyPrice;
                buyPrice = price;
            }
            
            ++it;
        }
        
        return profit;
        
    }
};
