//Say you have an array for which the ith element is the price of a given stock on day i.
//
//Design an algorithm to find the maximum profit. You may complete at most two transactions.
//
//Note:
//You may not engage in multiple transactions at the same time (ie, you must sell the stock before you buy agai
class Solution {
    
    typedef vector<int> list_type;
    typedef vector<int>::reverse_iterator riterator;
    
    // Find maximum profit when only one transaction is allowed in a day range
    int maxProfitSubRange(riterator rbegin, riterator rend)
    {
        int maxProfit = 0;
        int maxSellPrice = 0;
        
        riterator it = rbegin;
        while (it < rend) {
            int price = *it;
            if (price > maxSellPrice) {
                maxSellPrice = price;    
            }
            else {
                int profit = maxSellPrice - price;
                if (profit > maxProfit) {
                    maxProfit = profit;
                }
            }
            
            ++it;
        }
        
        return maxProfit;
    }
    
public:

    int maxProfit(vector<int> &prices) {
        if (prices.size() == 0) {
            return 0;
        }
        
        // bisect the day into two ranges and calculate max profit for each range
        int maxProfit = 0;
        
        riterator rightBegin = prices.rbegin();
        riterator leftEnd = prices.rend();
        riterator middle = rightBegin + 1;
        
        while (middle <= leftEnd) {
            int profitRight = maxProfitSubRange(rightBegin, middle);
            int profitLeft = maxProfitSubRange(middle, leftEnd);
            int profit = profitLeft + profitRight;
            
            if (profit > maxProfit) {
                maxProfit = profit;
            }
            
            ++middle;
        }
        
        return maxProfit;
    }
};
