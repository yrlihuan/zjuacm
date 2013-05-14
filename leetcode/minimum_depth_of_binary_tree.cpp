/**
 * Definition for binary tree
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    void searchRecursive(TreeNode *node, int depth, int &minDepth) {
        bool leaf = true;
        if (node->left) {
            leaf = false;
            searchRecursive(node->left, depth+1, minDepth);
        }
        
        if (node->right) {
            leaf = false;
            searchRecursive(node->right, depth+1, minDepth);
        }
        
        if (leaf) {
            minDepth = depth < minDepth ? depth : minDepth;
        }
    }
    
    int minDepth(TreeNode *root) {
        // Start typing your C/C++ solution below
        // DO NOT write int main() function
        if (!root) {
            return 0;
        }
        
        int minDepth = ~(1<<31);
        searchRecursive(root, 1, minDepth);
        
        return minDepth;
    }
};
