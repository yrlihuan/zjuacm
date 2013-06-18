/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    void removeNode(ListNode *node, ListNode **pNode) {
        *pNode = node->next;
        node->next = NULL;
    }
    
    void insertNode(ListNode *node, ListNode **pNode) {
        ListNode *next = (*pNode);
        node->next = next;
        *pNode = node;
    }
    
    ListNode *reverseKGroup(ListNode *head, int k) {
        ListNode *groupEnd = head;
        ListNode **pGroupHead = &head;
        while (true) {
            if (!groupEnd) {
                break;
            }
            
            int i = 0;
            for (; i < k-1; ++i) {
                ListNode *node = groupEnd->next;
                if (node == NULL) {
                    break;
                }
                
                removeNode(node, &(groupEnd->next));
                insertNode(node, pGroupHead);
            }
            
            if (i < k - 1) {
                groupEnd = *pGroupHead;
                for (int j = 0; j < i; ++j) {
                    ListNode *node = groupEnd->next;
                    removeNode(node, &(groupEnd->next));
                    insertNode(node, pGroupHead);
                }
                
                break;
            }
            
            pGroupHead = &(groupEnd->next);
            groupEnd = *pGroupHead;
        }
        
        return head;
    }
};
