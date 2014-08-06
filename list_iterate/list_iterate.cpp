#include <iostream>
#include <cstdlib>

using std::cout;
using std::endl;

class ListNode
{
public:
  ListNode():next(0) {}
  ListNode *next;
};

uint64_t rdtsc(){
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}

void create_list(int size, ListNode **head, ListNode **tail)
{
  *head = new ListNode();
  *tail = new ListNode();
  ListNode *node = *head;

  size -= 2;
  for (;size > 0; size--) {
    node->next = new ListNode();
    node = node->next;
  }

  node->next = *tail;
}

ListNode* algo1(ListNode *head, ListNode *tail)
{
  ListNode *pfast = head;
  ListNode *pslow = head;

  for (;;) {
    if (pfast != tail) {
      pfast = pfast->next;
    }
    else {
      break;
    }

    if (pfast != tail) {
      pfast = pfast->next;
    }
    else {
      break;
    }

    pslow = pslow->next;
  }

  return pslow;
}

ListNode* algo2(ListNode *head, ListNode *tail)
{
  ListNode *p = head;
  int cnt = 1;
  while (p != tail) {
    p = p->next;
    cnt++;
  }

  p = head;
  for (cnt = cnt / 2 - 1; cnt > 0; cnt--) {
    p = p->next;
  }

  return p;
}

void run_once()
{
  ListNode *head;
  ListNode *tail;
  ListNode *mid;

  create_list(10000, &head, &tail);

  uint64_t t1 = rdtsc();
  mid = algo1(head, tail);
  uint64_t t2 = rdtsc();

  cout << "  algo1:" << endl;
  cout << "    result: " << (void*)mid << endl;
  cout << "    ticks:  " << t2 - t1 << endl;

  t1 = rdtsc();
  mid = algo2(head, tail);
  t2 = rdtsc();

  cout << "  algo2:" << endl;
  cout << "    result: " << (void*)mid << endl;
  cout << "    ticks:  " << t2 - t1 << endl;
}

int main()
{
  int rounds = 5;
  for (int i = 1; i <= 5; ++i) {
    cout << "round: " << i << endl;
    run_once();
  }

  return 0;
}


