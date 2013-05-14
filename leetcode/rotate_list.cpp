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
    ListNode *rotateRight(ListNode *head, int k) {
        // Start typing your C/C++ solution below
        // DO NOT write int main() function
        
        // Use two run to do the rotation
        // first run: discover how many nodes are in the list
        int cnt = 0;
        ListNode *p = head;
        ListNode *oldTail = NULL;
        while (p) {
            oldTail = p;
            p = p->next;
            ++cnt;
        }
        
        if (cnt == 0) {
            // empty list
            return head;
        }
        
        // second run: get the new head/tail node
        k = k % cnt;
        if (k == 0) {
            return head;
        }
        
        int ind = 0;
        p = head;
        ListNode *newTail = oldTail;
        while (ind + k != cnt) {
            newTail = p;
            p = p->next;
            
            ++ind;
        }
        
        ListNode *newHead = p;
        newTail->next = (ListNode*)NULL;
        oldTail->next = head;
        
        return newHead;
    }
};
