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
    typedef vector<int>::iterator iter;

    TreeNode *buildSubTree(iter preBegin, iter preEnd, iter inBegin, iter inEnd)
    {
        if (preBegin == preEnd) {
            return (TreeNode*)NULL;
        }
        
        int rootVal = *preBegin;
        iter rootInIter = find(inBegin, inEnd, rootVal);
        
        int leftSize = rootInIter - inBegin;
        int rightSize = inEnd - rootInIter - 1;
        
        TreeNode *leftChild = buildSubTree(preBegin+1, preBegin+leftSize+1, inBegin, inBegin+leftSize);
        TreeNode *rightChild = buildSubTree(preBegin+leftSize+1, preEnd, rootInIter+1, inEnd);
        
        TreeNode *rootNode = new TreeNode(rootVal);
        rootNode->left = leftChild;
        rootNode->right = rightChild;
        
        return rootNode;
    }

    TreeNode *buildTree(vector<int> &preorder, vector<int> &inorder) {
        return buildSubTree(preorder.begin(), preorder.end(), inorder.begin(), inorder.end());
    }
};
