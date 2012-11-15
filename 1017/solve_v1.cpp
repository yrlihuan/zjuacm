#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <vector>
#include <iostream>
#include <set>
#include <string>

using namespace std;

#define MAXL 25
#define MAX_ROW (MAXL * 4 - 1)
#define UINT64MAX 0xffffffffffffffff
#define UINT1 (1UL)

typedef unsigned long long uint64;

class Digest
{
  public:
    Digest();
    Digest(uint64 high, uint64 low);

    bool bit_at(int pos);
    void set_bit(int pos);
    void clear_bit(int pos);

    Digest operator| (const Digest &rhs);
    Digest operator<< (int shift);
    bool operator< (const Digest &rhs) const;
    string to_s(int len);
  private:
    uint64 high_;
    uint64 low_;
};

Digest::Digest() : high_(0), low_(0)
{
}

Digest::Digest(uint64 high, uint64 low) : high_(high), low_(low)
{
}

bool Digest::bit_at(int pos)
{
  if (pos >= 64) {
    return high_ & (UINT1 << pos - 64);
  }
  else {
    return low_ & (UINT1 << pos);
  }
}

void Digest::set_bit(int pos)
{
  if (pos >= 64) {
    return high_ |= (UINT1 << pos - 64);
  }
  else {
    return low_ |= (UINT1 << pos);
  }
}

void Digest::clear_bit(int pos)
{
  if (pos >= 64) {
    return high_ &= ~(UINT1 << pos - 64);
  }
  else {
    return low_ &= ~(UINT1 << pos);
  }
}

bool Digest::operator< (const Digest &rhs) const
{
  return high_ < rhs.high_ || (high_ == rhs.high_ && low_ < rhs.low_);
}

string Digest::to_s(int len)
{
  char t[len+1];

  for (int p = 0; p < len; ++p) {
    uint64 bit = len-1-p >= 64 ? high_ & (UINT1 << len-1-p-64) : low_ & (UINT1 << len-1-p);
    t[p] = bit ? '+' : '-';
  }

  t[len] = 0;

  return string(t);
}

Digest Digest::operator| (const Digest &rhs)
{
  return Digest(high_ | rhs.high_, low_ | rhs.low_);
}

Digest Digest::operator<< (int shift)
{
  if (!shift)
    return *this;

  uint64 high = high_;
  uint64 low = low_;

  int shift2 = 64-shift;
  uint64 carry = shift2 > 0 ? low >> shift2 : low << -shift2;
  high = (shift2 > 0 ? high << shift : 0) | carry;

  low = shift2 > 0 ? low_ << shift : 0;

  return Digest(high, low);
}

int e;
int n_comp;
int comp[MAXL];
bool comp_valid[MAXL] = {false};

int table_resolved = 1;
vector<Digest> table[MAX_ROW];

void generate_transition_table(int len)
{
  // cout << "generate_table" << len << endl;
  if (len <= table_resolved) {
    return;
  }

  generate_transition_table(len - 2);

  for (int i = 0; i < n_comp; ++i) {
    int l = comp[i];
    int l2 = l * 2;
    int lpos = l2 - 1;

    if (l2 < len) {
      uint64 high = lpos > 64 ? UINT64MAX >> (128-lpos) : 0;
      uint64 low = lpos > 64 ? UINT64MAX : UINT64MAX >> (64-lpos);

      Digest appended(high, low);

      vector<Digest> &sub = table[len - l2];
      vector<Digest>::iterator it = sub.begin();
      while (it != sub.end()) {
        Digest tmp = (*it << l2) | appended;
        table[len].push_back(tmp);

        ++it;
      }
    }
    else if (l2-1 == len) {
      uint64 high = lpos > 64 ? UINT64MAX >> (128-lpos) : 0;
      uint64 low = lpos > 64 ? UINT64MAX : UINT64MAX >> (64-lpos);

      Digest appended(high, low);

      table[len].push_back(appended);
    }
  }

  table_resolved = len;
  // cout << "generate_table done" << len << " count: " << table[len].size() << endl;
}

void clean_data()
{
  memset(comp_valid, 0, sizeof(bool)*MAXL);

  for (int i = 1; i <= MAX_ROW; i+=2) {
    vector<Digest> &t = table[i];
    t.clear();
  }

  table_resolved = 1;
}

bool solve_recur(Digest d, int lvl)
{
}

vector<Digest> *next_levels(Digest d, int curlen, int nextlen)
{
  vector<Digest> *ret = new vector<Digest>();

  Digest dnext;

  if (curlen < nextlen) {
    bool pos_sec = false;
    for (int p = 0; p < curlen; ++p) {
      if (d.bit_at(p)) {
        if (pos_sec) {
          dnext.set_bit(p+1);
        }

        pos_sec = true;
      }
      else {
        if (pos_sec) {
          pos_sec = false;
          dnext.clear_bit(p);
        }
      }
    }
  }
  else if (curlen > nextlen) {
    bool pos_sec = true;
    for (int p = 1; p < curlen-1; ++p) {
      if (d.bit_at(p)) {
        if (pos_sec) {
          dnext.set_bit(p-1);
        }

        pos_sec = true;
      }
      else {
        if (pos_sec) {
          pos_sec = false;
          dnext.clear_bit(p);
        }
      }
    }

  }
  else {

  }

  for (int p = 0; p < curlen; ++p) {
    bool bit = d.bit_at(p);
    }
  }
  Digest dnext;
  

  return ret;
}

bool solve_case()
{
  cin >> e >> n_comp;

  int i = 0;
  for (i = 0; i < n_comp; ++i) {
    cin >> comp[i];
    comp_valid[comp[i]] = true;

    if (e % comp[i] == 0) {
      return true;
    }
  }

  set<Digest> states1;
  set<Digest> states2;
  for (int row = 0; row < e*2; ++row) {
    set<Digest> *cur;
    set<Digest> *prev;
    if (row % 2) {
      cur = &states1;
      prev = &states2;
    }
    else {
      cur = &states2;
      prev= &states1;
    }

    int curlen = row < e ? e*2+1+row*2 : e*6-1-row*2;
    int prevlen = row < e ? curlen-2 : (row > e ? curlen+2 : curlen);

    if (row == 0) {
      generate_transition_table(curlen-2);

      vector<Digest> &t = table[curlen-2];
      vector<Digest>::iterator it = t.begin();

      while (it != t.end()) {
        cur->insert(*it << 1);
        ++it;
      }
    }
    else {
      set<Digest>::iterator prev_it = prev.begin();
      while (prev_it != prev.end()) {


        prev_it++;
      }
    }

    prev->clear();
  }

  clean_data();

  return false;
}

int main()
{
  int n;
  cin >> n;
  for (int i = 0; i < n; ++i) {
    if (solve_case()) {
      cout << "YES" << endl;
    }
    else {
      cout << "NO" << endl;
    }
  }

  // Digest d(UINT64MAX, UINT64MAX);
  // cout << (d << 1).to_s(128) << endl;
}

