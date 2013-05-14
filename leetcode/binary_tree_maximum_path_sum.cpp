/**
 * Definition for binary tree
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
 
typedef struct NodeInfoType {
    int maxSubTreesSum;
    int maxPathSum;
} NodeInfo;

class Solution {
public:

    NodeInfo maxSumRecursive(TreeNode *root) {
        if (!root) {
            NodeInfo info;
            info.maxPathSum = 0;
            info.maxSubTreesSum = -65536;
            
            return info;
        }
        
        NodeInfo leftInfo = maxSumRecursive(root->left);
        NodeInfo rightInfo = maxSumRecursive(root->right);
        
        NodeInfo info;
        
        int leftPath = leftInfo.maxPathSum > 0 ? leftInfo.maxPathSum : 0;
        int rightPath = rightInfo.maxPathSum > 0 ? rightInfo.maxPathSum : 0;
        info.maxPathSum = root->val + (leftPath > rightPath ? leftPath : rightPath);
        
        info.maxSubTreesSum = root->val + leftPath + rightPath;
        info.maxSubTreesSum = info.maxSubTreesSum > leftInfo.maxSubTreesSum ? info.maxSubTreesSum : leftInfo.maxSubTreesSum;
        info.maxSubTreesSum = info.maxSubTreesSum > rightInfo.maxSubTreesSum ? info.maxSubTreesSum : rightInfo.maxSubTreesSum;
        
        return info;
    }
    

    int maxPathSum(TreeNode *root) {
        // Start typing your C/C++ solution below
        // DO NOT write int main() function
        
        NodeInfo info = maxSumRecursive(root);
        return info.maxSubTreesSum;
    }
};
