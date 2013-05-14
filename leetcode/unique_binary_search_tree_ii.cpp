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
    map<pair<int,int>, vector<TreeNode*>> cache;
    bool initialized = false;
    
    TreeNode *deepCopy(TreeNode *node, int delta) {
        if (node == NULL) {
            return NULL;
        }
        else {
            TreeNode *newNode = new TreeNode(node->val + delta);
            newNode->left = deepCopy(node->left, delta);
            newNode->right = deepCopy(node->right, delta);
            
            return newNode;
        }
    }

    // helper function to dynamic programming and node reuse
    // caller should ensure all calls happen in right order
    vector<TreeNode*> getSubTrees(int size, int start) {
        if (!initialized) {
            vector<TreeNode*> nullVector(1, (TreeNode*)NULL);
            cache[make_pair(0,0)] = nullVector;
            
            vector<TreeNode*> oneVector(1, new TreeNode(0));
            cache[make_pair(1,0)] = oneVector;
            
            initialized = true;
        }
        
        // it's in the cache
        if (cache.find(make_pair(size, start)) != cache.end()) {
            return cache[make_pair(size, start)];
        }
        // or we can easily create a copy from a previous tree
        else if (cache.find(make_pair(size, 0)) != cache.end()) {
            vector<TreeNode*> &oldVector = cache[make_pair(size, 0)];
            
            vector<TreeNode*> result;
            vector<TreeNode*>::iterator it = oldVector.begin();
            while (it != oldVector.end()) {
                result.push_back(deepCopy(*it, start-0));
                ++it;
            }
            
            cache[make_pair(size, start)] = result;
            return result;
        }
        // means the size is new, ignore start for now and assume it's 0
        else {
            vector<TreeNode*> result;
            
            
            for (int i = 0; i < size; ++i) {
                vector<TreeNode*> l = getSubTrees(i, 0);
                vector<TreeNode*> r = getSubTrees(size-i-1, i+1);
                
                vector<TreeNode*>::iterator itl = l.begin();
                while (itl != l.end()) {
                    vector<TreeNode*>::iterator itr = r.begin();
                    while (itr != r.end()) {
                        
                        TreeNode *newRoot = new TreeNode(i);
                        newRoot->left = *itl;
                        newRoot->right = *itr;
                        
                        result.push_back(newRoot);
                        
                        ++itr;
                    }
                    
                    ++itl;
                }
                
                TreeNode *root = new TreeNode(i);
            }
            
            cache[make_pair(size, 0)] = result;
            if (start == 0) {
                return result;
            }
            else {
                return getSubTrees(size, start);
            }
        }
        
        
    }

    vector<TreeNode *> generateTrees(int n) {
        return getSubTrees(n, 1);
    }
};
