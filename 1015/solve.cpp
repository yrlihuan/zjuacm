#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <set>
#include <map>
#include <vector>
#include <list>
#include <iostream>

using namespace std;

#define MAXN 1000

vector<int> lex_bfs(int n, bool map[][MAXN])
{
  // cout << "lex_bfs" << endl;
  list<list<int>*> remaining;

  list<int> *s = new list<int>();
  for (int i = 0; i < n; ++i) {
    s->push_back(i);
  }

  remaining.push_back(s);

  vector<int> lex_order;
  while (remaining.size()) {

    list<int> *first_set = *(remaining.begin());
    int s = first_set->front();
    first_set->pop_front();

    lex_order.push_back(s);

    list<list<int>*>::iterator it_set = remaining.begin();
    while (it_set != remaining.end()) {
      bool set_copied = false;
      list<int> *next_set = *it_set;
      list<int>::iterator it = next_set->begin();

      while (it != next_set->end()) {
        int d = *it;
        if (map[s][d]) {
          if (!set_copied) {
            remaining.insert(it_set, new list<int>());
            set_copied = true;
          }

          (*--it_set)->push_back(d);
          it = (*++it_set)->erase(it);
        }
        else {
          ++it;
        }
      }

      ++it_set;
    }

    it_set = remaining.begin();
    while (it_set != remaining.end()) {
      list<int> *next_set = *it_set;
      if (next_set->size() == 0) {
        it_set = remaining.erase(it_set);
        delete next_set;
      }
      else {
        ++it_set;
      }
    }
  }

  return lex_order;
}

bool solve(int n, int m)
{
  bool map[MAXN][MAXN];
  memset(map, 0, sizeof(bool)*MAXN*MAXN);

  int i;
  for (i = 0; i < m; ++i) {
    int a, b;
    cin >> a >> b;

    --a;
    --b;
    map[a][b] = true;
    map[b][a] = true;
  }

  vector<int> lex_order = lex_bfs(n, map);
  vector<int> reversed(lex_order.rbegin(), lex_order.rend());

  //vector<int>::iterator it = lex_order.begin();
  //while (it != lex_order.end()) {
  //  cout << *it << endl; //  ++it;
  //}

  //cout << endl;

  //it = reversed.begin();
  //while (it != reversed.end()) {
  //  cout << *it << endl;
  //  ++it;
  //}

  //cout << endl;

  vector<int>::iterator v = reversed.begin();
  while (v < reversed.end()) {
    vector<int>::iterator w = v + 1;
    while (w < reversed.end()) {
      if (map[*v][*w]) {
        break;
      }

      ++w;
    }

    if (w < reversed.end()) {
      vector<int>::iterator c = w + 1;
      while (c < reversed.end()) {
        if (map[*v][*c] && !map[*w][*c]) {
          return false;
        }

        ++c;
      }
    }

    ++v;
  }

  return true;
}

int main()
{
  int n, m;
  cin >> n >> m;
  while (n) {
    if (solve(n, m)) {
      cout << "Perfect" << endl << endl;
    }
    else {
      cout << "Imperfect" << endl << endl;
    }

    cin >> n >> m;
  }
}
