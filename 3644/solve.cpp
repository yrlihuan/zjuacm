#include <stdio.h>
#include <vector>

using namespace std;

int solutions = 0;
int target = -1;
int target_n = -1;
int current_lcm = -1;
int *scores = NULL;
int *valid_points = NULL;
vector<int> *step_cache = NULL;
vector<int> **conn = NULL;

int gcd(int a, int b)
{
  while(b)
  {
    int c = b;
    b = a % b;
    a = c;
  }

  return a;
}

int lcm(int a, int b)
{
  return (long long)a * b / gcd(a, b);
}

int scan_data()
{
  delete []scores;
  delete []valid_points;
  delete step_cache;

  for (int i = 0; i < target_n; ++i) {
    if (conn) {
      delete conn[i];
    }
  }

  delete []conn;

  int m;
  if (scanf("%d %d %d",&target_n, &m, &target) == EOF) {
    return 0;
  }

  scores = new int[target_n];
  valid_points = new int[target_n];
  step_cache = new vector<int>();
  conn = new vector<int>*[target_n];

  current_lcm = 1;

  for (int i = 0; i < target_n; ++i) {
    conn[i] = new vector<int>();
    step_cache->push_back(0);
  }

  for (int i = 0; i < m; ++i) {
    int f, t;
    scanf("%d %d", &f, &t);

    conn[--f]->push_back(--t);
  }

  for (int i = 0; i < target_n; ++i) {
    scanf("%d", scores + i);

    valid_points[i] = scores[i] == gcd(scores[i], target);
  }

  return 1;
}

int visit(int p)
{
  //printf("visit: %d\n", p);
  int value = lcm(current_lcm, scores[p]);
  step_cache->push_back(value);
  current_lcm = value;

  int l = step_cache->size();
  if ((*step_cache)[l-1] == (*step_cache)[l-2]) {
    return false;
  }

  //printf("value: %d\n", value);
  if (p == target_n-1) {
    if (value == target) {
      ++solutions;
    }

    return false;
  }

  return gcd(value, target) == value;
}

void rollback(int p)
{
  //printf("rollback: %d\n", p);
  step_cache->pop_back();
  current_lcm = (*step_cache)[step_cache->size()-1];
}

void graph_walk()
{
  int visited[target_n];
  int search_index[target_n];
  for (int i = 0; i < target_n; ++i) {
    visited[i] = false;
    search_index[i] = -1;
  }

  vector<int> path = vector<int>();

  int p = 0;
  while (true) {
    visited[p] = true;
    path.push_back(p);
    int valid = visit(p);
    int next_p = -1;

    if (valid) {
      vector<int> *candidates = conn[p];
      int ind = 0;
      while (candidates->size() > ind) {
        int c = candidates->at(ind);
        if (!visited[c] && valid_points[c]) {
          next_p = c;
          search_index[p] = ind;
          break;
        }
        else {
          ++ind;
        }
      }
    }

    if (valid && next_p >= 0) {
      p = next_p;
    }
    else {
      int rollback_done = false;
      visited[p] = false;
      path.pop_back();
      rollback(p);

      while (path.size() > 0) {
        int last_p = path[path.size()-1];
        vector<int> *candidates = conn[last_p];
        int ind = search_index[last_p] + 1;

        while (candidates->size() > ind) {
          int c = candidates->at(ind);
          if (!visited[c] && valid_points[c]) {
            search_index[last_p] = ind;
            p = c;
            rollback_done = true;
            break;
          }
        }

        if (rollback_done)
          break;
        else {
          search_index[last_p] = -1;
          visited[last_p] = false;
          path.pop_back();
          rollback(last_p);
        }
      }

      if (!rollback_done) {
        break;
      }
    }
  }
}

int main()
{
  while(scan_data()) {
    solutions = 0;
    graph_walk();

    printf("%d\n", solutions % 1000000007);
  }

  return 0;
}
