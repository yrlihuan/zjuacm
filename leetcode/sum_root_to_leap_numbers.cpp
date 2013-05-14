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
    int sumNumbers(TreeNode *root) {
        // Start typing your C/C++ solution below
        // DO NOT write int main() function
        int sum = 0;
        
        if (!root) {
            return 0;
        }
        
        
        // run a depth first search
        vector<TreeNode*> path;
        int value = 0;
        path.push_back(root);
        
        while (path.size() > 0) {
            TreeNode *current = path.back();
            
            if (current->left) {
                // search left child
                path.push_back(current->left);
                value = value * 10 + current->val;
            } 
            else if (current->right) {
                // search right child
                path.push_back(current->right);
                value = value * 10 + current->val;
            }
            else {
                // a leaf node
                value = value * 10 + current->val;
                sum += value;
                
                // rewind
                path.pop_back();
                while (path.size() > 0) {
                    TreeNode *parent = path.back();
                    value /= 10;
                    
                    if (parent->left == current && parent->right) {
                        path.push_back(parent->right);
                        break;
                    }
                    else {
                        path.pop_back();
                        current = parent;
                    }
                }
            }
        }
        
        return sum;
    }
};
