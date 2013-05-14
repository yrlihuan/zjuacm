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
    typedef pair<int, int> PV;
    void traversal(TreeNode *p, int lvl, vector<PV> &preorder, vector<PV> &inorder) {
       preorder.push_back(make_pair(lvl, p->val));
       
       if (p->left) {
           traversal(p->left, lvl+1, preorder, inorder);
       }
       
       inorder.push_back(make_pair(lvl, p->val));
       
       if (p->right) {
           traversal(p->right, lvl+1, preorder, inorder);
       }
    }

    bool isSameTree(TreeNode *p, TreeNode *q) {
        if (p == NULL && q == NULL) {
            return true;
        }
        
        if (p == NULL || q == NULL) {
            return false;
        }
        
        // Start typing your C/C++ solution below
        // DO NOT write int main() function
        typedef vector<PV> VI;
        VI pPreorder;
        VI pInorder;
        
        VI qPreorder;
        VI qInorder;
        
        traversal(p, 0, pPreorder, pInorder);
        traversal(q, 0, qPreorder, qInorder);
        
        return pPreorder == qPreorder && pInorder == qInorder;
    }
};
