#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <set>
#include <map>
#include <vector>
#include <list>
#include <iostream>

using namespace std;

typedef struct PointType
{
  int x;
  int y;
} Point;

int solve_case()
{
  int m, n;
  cin >> m >> n;

  int grids[m][n];
  int i, j;
  for (i = 0; i < m; ++i) {
    for (j = 0; j < n; ++j) {
      cin >> grids[i][j];
    }
  }

  cin >> i >> j;
  vector<int> lower;
  vector<int> upper;
  vector<char> direction;

  while (i && j) {
    char d;
    cin >> d;

    lower.push_back(i);
    upper.push_back(j);
    direction.push_back(d);

    cin >> i >> j;
  }

  char pos[m][n];
  memset(pos, 1, sizeof(pos));

  int ind = lower.size();
  while (ind-- > 0) {
    i = lower[ind];
    j = upper[ind];
    char d = direction[ind];

    int dx = 0;
    int dy = 0;

    if (d == 'L') {
      dx = 1;
    }
    else if (d == 'R') {
      dx = -1;
    }
    else if (d == 'U') {
      dy = 1;
    }
    else {
      dy = -1;
    }

    int p;
    char pos_next[m][n];
    memset(pos_next, 0, sizeof(pos_next));

    for (p = 0; p < m * n; ++p) {
      int y = p/n;
      int x = p%n;
      if (!pos[y][x] || grids[y][x]) {
        continue;
      }

      int step = 0;
      while (++step <= j) {
        y += dy;
        x += dx;

        if (x < 0 || x >= n || y < 0 || y >= m || grids[y][x]) {
          break;
        }

        if (step >= i) {
          pos_next[y][x] = 1;
        }
      }
    }

    //for (int s = 0; s < m; ++s) {
    //  for (int t = 0; t < n; ++t) {
    //    cout << (pos_next[s][t] ? '*' : grids[s][t] ? '1' : '-') << ' ';
    //  }
    //  cout << endl;
    //}
    //cout << endl;

    memcpy(pos, pos_next, sizeof(pos));
  }

  int cnt = 0;
  for (int p = 0; p < m * n; ++p) {
    int y = p/n;
    int x = p%n;

    cnt += pos[y][x] && !grids[y][x] ? 1 : 0;
  }

  return cnt;
}

int main()
{
  int cases = 0;
  cin >> cases;

  while (cases--) {
    cout << solve_case() << endl;
  }
}
