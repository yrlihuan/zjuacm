/**
 * Definition for binary tree with next pointer.
 * struct TreeLinkNode {
 *  int val;
 *  TreeLinkNode *left, *right, *next;
 *  TreeLinkNode(int x) : val(x), left(NULL), right(NULL), next(NULL) {}
 * };
 */
class Solution {
public:
    TreeLinkNode *connectLevel(TreeLinkNode *node) {
        TreeLinkNode *levelStart = NULL;
        TreeLinkNode *pos = NULL;
        
        while (node) {
            TreeLinkNode *children[] = {node->left, node->right};
            for (int i = 0; i < 2; ++i) {
                TreeLinkNode *child = children[i];
                if (!child) {
                    continue;
                }
                
                if (!levelStart) {
                    levelStart = child;
                }
                else {
                    pos->next = child;
                }
                
                pos = child;
            }
            
            node = node->next;
        }
        
        if (pos) {
            pos->next = NULL;
        }
        
        return levelStart;
    }

    void connect(TreeLinkNode *root) {
        if (root == NULL) {
            return;
        }
        
        root->next = NULL;
        TreeLinkNode *prevLevel = root;
        TreeLinkNode *curLevel;
        
        while (true) {
            curLevel = connectLevel(prevLevel);
            if (!curLevel) {
                break;
            }
            
            prevLevel = curLevel;
        }
    }
};
