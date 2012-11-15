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
#define PIVOT MAXL
#define MAX_ROW (MAXL*2+PIVOT)
#define MAX_COL (MAXL*4-1+PIVOT*2)

int e;
int n;
bool map[MAX_ROW][MAX_COL];
int comp[MAXL];

int row_start(int row)
{
  if (row < e) {
    return e - row - 1 + PIVOT;
  }
  else {
    return row - e + PIVOT;
  }
}

void init_map()
{
  memset(map, 0, sizeof(bool)*MAX_ROW*MAX_COL);

  for (int r = 0; r < e*2; ++r) {
    int s = 0;
    if (r < e) {
      s = e - r - 1;
    }
    else {
      s = r - e;
    }

    for (int c = 0; c < MAX_COL; ++c) {
      if (c >= PIVOT+s && c < PIVOT+4*e-1-s) {
        map[r][c] = true;
      }
    }
  }

  //for (int i = 0; i < 2*e; ++i) {
  //  for (int j = PIVOT; j < 4*e-1+PIVOT; ++j) {
  //    cout << map[i][j] ? 1 : 0;
  //  }
  //  cout << endl;
  //}
}

int comp_compare(const void *a, const void *b)
{
  return *(int*)a - *(int*)b;
}

bool can_place(int row, int col, int size)
{
  //printf("test: %d, %d  %d\n", row, col, size);
  if (row + size > 2*e) {
    return false;
  }

  // style 1: growing triangle
  // style 0: shrinking triangle
  int style = (row + col + e + PIVOT) & 0x1;
  if (style == 1) {
    int left = col;
    int right = left;
    for (int i = 0; i < size; ++i) {
      if (!map[row][left] || !map[row][right]) {
        return false;
      }

      row++;
      left--;
      right++;
    }

    return true;
  }
  else {
    int left = col;
    int right = left + 2*size - 2;
    return map[row][left] && map[row][right];
  }
}

int place(int row, int col, int size, bool fill)
{
  //if (!fill) {
  //  printf("placing %d: %d, %d\n", size, row, col);
  //}
  //else {
  //  printf("undoing %d: %d, %d\n", size, row, col);
  //}

  // style 1: growing triangle
  // style 0: shrinking triangle
  int style = (row + col + e + PIVOT) & 0x1;
  if (style == 1) {
    int left = col;
    int right = left;
    for (int i = 0; i < size; ++i) {
      for (int c = left; c <= right; ++c) {
        map[row][c] = fill;
      }

      row++;
      left--;
      right++;
    }

    return 1;
  }
  else {
    int left = col;
    int right = left + 2*size - 2;
    for (int i = 0; i < size; ++i) {
      for (int c = left; c <= right; ++c) {
        map[row][c] = fill;
      }

      row++;
      left++;
      right--;
    }

    return 2*size-1;
  }
}

bool search(int row, int col)
{
  if (!map[row][col]) {
    while (row < 2*e && !map[row][col]) {
      col++;
      if (col >= MAX_COL - PIVOT) {
        col = row_start(++row);
      }
    }

    // printf("moving to %d %d\n", row, col);
  }

  if (row >= 2 * e) {
    return true;
  }

  int s;
  if (row < e) {
    s = e - row - 1 + PIVOT;
  }
  else {
    s = row - e + PIVOT;
  }

  int right = MAX_COL - s;

  for (int i = 0; i < n; ++i) {
    int s = comp[i];
    if (can_place(row, col, s)) {
      int c2 = col + place(row, col, s, false);
      bool sub = search(row, c2);
      //bool sub = false;
      if (sub) {
        return true;
      }

      place(row, col, s, true);
    }
    else {
      break;
    }
  }

  return false;
}

bool solve_case()
{
  cin >> e >> n;

  int i = 0;

  for (i = 0; i < n; ++i) {
    cin >> comp[i];

    if (e % comp[i] == 0) {
      return true;
    }
  }

  qsort(comp, n, sizeof(int), comp_compare);

  int pos = 0;
  for (i = 0; i < n; ++i) {
    int c = comp[i];
    bool redundent = false;
    for (int j = 0; j < pos; ++j) {
      if (c % comp[j] == 0) {
        redundent = true;
        break;
      }
    }

    if (!redundent) {
      comp[pos++] = c;
    }
  }

  //cout << "------------------ CASE START ------------------" << endl;

  n = pos;
  init_map();
  return search(0, row_start(0));
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
}

