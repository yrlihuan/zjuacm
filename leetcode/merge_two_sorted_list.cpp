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
    ListNode *mergeTwoLists(ListNode *l1, ListNode *l2) {
        // Start typing your C/C++ solution below
        // DO NOT write int main() function
        
        if (l1 == NULL) {
            return l2;
        }
        
        if (l2 == NULL) {
            return l1;
        }
        
        ListNode *p1 = l1;
        ListNode *p2 = l2;
        ListNode *result = NULL;
        ListNode *p = NULL;
        
        if (p1->val < p2->val) {
            result = p1;
            p1 = p1->next;
        }
        else {
            result = p2;
            p2 = p2->next;
        }
        
        p = result;
        while (p1 != NULL && p2 != NULL) {
            if (p1->val < p2->val) {
                p->next = p1;
                p1 = p1->next;
            }
            else {
                p->next = p2;
                p2 = p2->next;
            }
            
            p = p->next;
        }
        
        while (p1 != NULL) {
            p->next = p1;
            p1 = p1->next;
            p = p->next;
        }
        
        while (p2 != NULL) {
            p->next = p2;
            p2 = p2->next;
            p = p->next;
        }
        
        p->next = (ListNode*)NULL;
        
        return result;
        
    }
};
