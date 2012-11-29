#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <set>
#include <map>
#include <vector>
#include <list>
#include <iostream>
#include <algorithm>

using namespace std;

#define INVALID_VOL 655360

class Link
{
public:
  int lind;
  int rind;
  int y;
};

class Pipe
{
public:
  int ind;
  int top;
  int h;

  int level;

  int src_h;
  vector<Pipe>::iterator parent;

  vector<int> links;
  vector<int> link_heights;
};

typedef vector<Pipe> vecP;
typedef vector<Pipe>::iterator iterP;
typedef vector<int>::iterator iterI;

int calc_level(const Pipe &p, int parent_lvl)
{
  if (parent_lvl > p.src_h) {
    return p.top + p.h + 1;
  }
  else if (parent_lvl > p.top) {
    return parent_lvl;
  }
  else {
    return p.top;
  }
}

iterP get_pipe(int ind, vecP &pipes)
{
  iterP it = pipes.begin();
  while (it != pipes.end()) {
    if (it->ind == ind) {
      return it;
    }

    ++it;
  }

  return it;
}

int solve_case(int target, int th, vecP &pipes)
{
  // 1st, build connection graph
  vector<iterP> seq;
  iterP next = pipes.end();
  int next_h = -1;

  pipes[0].parent = pipes.end();
  while (seq.size() < pipes.size()) {
    next_h = -1;

    iterP it = pipes.begin();
    while (it < pipes.end()) {
      if (find(seq.begin(), seq.end(), it) == seq.end() && it->src_h > next_h) {
        next_h = it->src_h;
        next = it;
      }

      ++it;
    }

    if (next_h == -1) {
      break;
    }

    seq.push_back(next);

    iterI pLink = next->links.begin();
    iterI pHeight = next->link_heights.begin();
    while (pLink != next->links.end()) {
      iterP child = get_pipe(*pLink, pipes);
      if (find(seq.begin(), seq.end(), child) != seq.end()) {
        pLink++;
        pHeight++;
        continue;
      }

      // Here we have some problems
      if (child->src_h < *pHeight) {
        child->src_h = *pHeight;
        child->parent = next;
      }

      pLink++;
      pHeight++;
    }

    //it = pipes.begin();
    //while (it != pipes.end()) {
    //  cout << it->src_h << endl;
    //  ++it;
    //}
  }

  // run...
  int cost = 0;
  iterP target_pipe = pipes.begin() + target;

  if (th > target_pipe->top + target_pipe->h || th <= target_pipe->top) {
    return INVALID_VOL;
  }

  while (cost < INVALID_VOL && target_pipe->level > th) {
    seq[0]->level--;
    bool exceeded = false;
    for (int i = 1; i < seq.size(); ++i) {
      iterP p = seq[i];
      iterP parent = p->parent;

      int level = calc_level(*p, parent->level);

      if (level != p->level) {
        if (p->ind == target_pipe->ind && level < th) {
          p->level = th;
          exceeded = true;
        }
        else {
          p->level = level;
        }
      }
    }

    cost = 0;
    for (int i = 0; i < seq.size(); ++i) {
      iterP p = seq[i];
      iterP pp = p;

      bool targets_child = false;
      while (pp->parent != pipes.end()) {
        if (pp->parent == target_pipe) {
          targets_child = true;
          break;
        }

        pp = pp->parent;
      }

      if (p->level > p->top + p->h) {
        cost += 0;
      }
      else if (p->level > p->top || (exceeded && !targets_child)) {
        cost += p->top + p->h - p->level;
      }
      else {
        cost += INVALID_VOL;
      }
    }

    //cout << "------------" << endl;
    //for (int i = 0; i < seq.size(); ++i) {
    //  cout << seq[i]->level << endl;
    //}
    //cout << cost << endl;
  }

  return cost;
}

int main()
{
  int cases;

  cin >> cases;

  while (cases--) {
    int p;
    cin >> p;

    vecP pipes;
    for (int i = 0; i < p; ++i) {
      Pipe pipe;
      cin >> pipe.ind >> pipe.top >> pipe.h;

      if (!i) {
        pipe.src_h = pipe.top + pipe.h + 1;
      }
      else {
        pipe.src_h = -1;
      }

      pipe.level = pipe.top + pipe.h + 1;

      pipes.push_back(pipe);
    }

    int l;
    cin >> l;
    for (int i = 0; i < l; ++i) {
      int x, y, len;
      cin >> x >> y >> len;
      Link link;
      link.lind = x - 1;
      link.rind = x + len;
      link.y = y;

      iterP it = pipes.begin();
      while (it != pipes.end()) {
        if (it->ind == link.lind) {
          it->links.push_back(link.rind);
          it->link_heights.push_back(link.y);
        }
        else if (it->ind == link.rind) {
          it->links.push_back(link.lind);
          it->link_heights.push_back(link.y);
        }

        ++it;
      }
    }

    int target, th;
    cin >> target >> th;

    int result = solve_case(target-1, th, pipes);
    if (result < INVALID_VOL) {
      cout << result << endl;
    }
    else {
      cout << "No Solution\n";
    }
  }
}

