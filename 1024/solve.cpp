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

int to_ind(int y, int m, int d)
{
  return 32*12*(y-1900) + 32*(m-1) + (d-1);
}

void to_date(int ind, int *y, int *m, int *d)
{
  *d = ind % 32 + 1;
  *m = ind / 32 % 12 + 1;
  *y = 1900 + ind / 32 / 12;
}

int next_calendar_day(int ind)
{
  int mdays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  int y, m, d;
  to_date(ind + 1, &y, &m, &d);

  if (d <= mdays[m-1]) {
    return ind + 1;
  }

  if (m == 2 && d == 29 && y % 4 == 0 && (y % 100 != 0 || y % 400 == 0)) {
    return ind + 1;
  }

  d = 1;
  if (m == 12) {
    m = 1;
    y++;
  }
  else {
    m++;
  }

  return to_ind(y, m, d);
}

int prev_calendar_day(int ind)
{
  int mdays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  int y, m, d;
  to_date(ind, &y, &m, &d);
  //printf("%d, %d, %d\n", y, m, d);

  if (d > 1) {
    return ind - 1;
  }

  if (m == 1) {
    m = 12;
    y--;
  }
  else {
    m--;
  }

  d = mdays[m-1];
  if (m == 2 && y % 4 == 0 && (y % 100 != 0 || y % 400 == 0)) {
    d++;
  }

  return to_ind(y, m, d);
}

int day_next_month(int ind)
{
  return ind + 32;
}

int day_prev_month(int ind)
{
  return ind - 32;
}

int main()
{
  char *flags = new char[32*12*110];
  memset(flags, -1, 32*12*110);

  int ind = to_ind(2001, 11, 4);
  flags[ind] = 1;

  int cnt = 1;
  while (ind) {
    ind = prev_calendar_day(ind);
    // cout << ind << endl;
    flags[ind] = 0;

    cnt++;
  }

  // cout << cnt << endl;

  ind = to_ind(2001, 11, 4);
  while (ind) {
    ind = prev_calendar_day(ind);

    int d1 = next_calendar_day(ind);
    int d2 = day_next_month(ind);

    if (flags[d1] == 1 || flags[d2] == 1) {
      flags[ind] = -1;
    }
    else if (flags[d1] == -1 && flags[d2] == -1) {
      flags[ind] = 1;
    }
    else {
      flags[ind] = 0;
    }
  }

  int n;
  cin >> n;

  while (n--) {
    int y, m, d;
    cin >> y >> m >> d;

    int ind = to_ind(y, m, d);
    cout << (flags[ind] == -1 ? "YES" : "NO") << endl;
  }
}
