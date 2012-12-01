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

#define MAXN (32*12*700)
int offset[MAXN];

int to_ind(int y, int m, int d)
{
  return 32*12*(y-1700) + 32*(m-1) + (d-1);
}

void to_date(int ind, int *y, int *m, int *d)
{
  *d = ind % 32 + 1;
  *m = ind / 32 % 12 + 1;
  *y = 1700 + ind / 32 / 12;
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

vector<int*> perm3;
void perm_gen(int list[], int i, int n)
{
  int j;
  if(i==n)
  {
    int *p = new int[n];
    memcpy(p, list, sizeof(int)*n);

    perm3.push_back(p);
  }
  else
  {
    for(j=i;j<n;j++)
    {
        swap(list[i],list[j]);
        perm_gen(list,i+1,n);
        swap(list[i],list[j]);
    }
  }
}

void init_offsets()
{
  memset(offset, 0, sizeof(offset));
  int pivot = to_ind(2001, 11, 4);
  int max = to_ind(2299, 12, 31);
  int min = to_ind(1700, 1, 1);

  int cnt = 0;
  int ind = pivot;
  while (ind < max) {
    ind = next_calendar_day(ind);
    offset[ind] = ++cnt;
  }

  cnt = 0;
  ind = pivot;
  while (ind > min) {
    ind = prev_calendar_day(ind);
    offset[ind] = --cnt;
  }
}

vector<int> ind_for_candidate(int parts[], int lengths[])
{
  vector<int> result;

  vector<int*>::iterator p = perm3.begin();
  while (p < perm3.end()) {
    int y = parts[(*p)[0]];
    int m = parts[(*p)[1]];
    int d = parts[(*p)[2]];

    int yl = lengths[(*p)[0]];
    int ml = lengths[(*p)[1]];
    int dl = lengths[(*p)[2]];

    ++p;

    if (yl != 1 && yl != 2 && yl != 4) {
      continue;
    }
    else if (ml > 2) {
      continue;
    }
    else if (dl > 2) {
      continue;
    }
    else if (y >= 100 && (y < 1700 || y > 2299)) {
      continue;
    }
    else if (m < 1 || m > 12) {
      continue;
    }
    else if (d < 1 || d > 31) {
      continue;
    }
    else if (y < 100 && yl == 4) {
      continue;
    }
    else {
      if (y >= 100) {
        int ind = to_ind(y, m, d);
        int of = offset[ind];
        if (of != 0 || (y == 2011 && m == 11 && d == 4)) {
          result.push_back(of);
        }
      }
      else {
        for (y = 1700+y; y < 2300; y+=100) {
          int ind = to_ind(y, m, d);
          int of = offset[ind];
          if (of != 0 || (y == 2011 && m == 11 && d == 4)) {
            result.push_back(of);
          }
        }
      }
    }
  }

  return result;
}

void solve_case()
{
  char date[32];
  cin >> date;

  char delimiter = '\0';
  char *p = date;
  while (*p != '\0') {
    if (*p < '0' || *p > '9') {
      delimiter = *p;
      break;
    }

    ++p;
  }

  set<int> indices;
  char format[] = "";
  if (delimiter != '\0') {
    char f[32];
    sprintf(f, "%%d%c%%d%c%%d", delimiter, delimiter);

    int parts[3];
    int lengths[3];
    sscanf(date, f, parts, parts+1, parts+2);

    int pstart = 0;
    int p = 0;
    int j = 0;
    while (true) {
      if (date[p] == delimiter || date[p] == '\0') {
        lengths[j++] = p - pstart;
        pstart = p + 1;
      }

      if (date[p] == '\0') {
        break;
      }

      ++p;
    }

    vector<int> inds = ind_for_candidate(parts, lengths);

    indices.insert(inds.begin(), inds.end());
  }
  else {
    delimiter = '-';
    int l = strlen(date);
    char date2[32];
    for (int p1 = 1; p1 < l; ++p1) {
      for (int p2 = p1+1; p2 < l; ++p2) {
        char *pdate2 = date2;
        p = date;

        while (*p != '\0') {
          if (p - date == p1 || p - date == p2) {
            *(pdate2++) = delimiter;
          }

          *(pdate2++) = *(p++);
        }

        *pdate2 = '\0';

        char f[32];
        sprintf(f, "%%d%c%%d%c%%d", delimiter, delimiter);

        int parts[3];
        int lengths[3];
        sscanf(date2, f, parts, parts+1, parts+2);

        lengths[0] = p1;
        lengths[1] = p2 - p1;
        lengths[2] = l - p2;
        vector<int> inds = ind_for_candidate(parts, lengths);

        //cout << date2 << endl;
        //cout << inds.size() << endl;
        //for (int i = 0; i < 3; ++i) {
        //  cout << parts[i] << endl;
        //}

        indices.insert(inds.begin(), inds.end());
      }
    }
  }

  set<int>::iterator it = indices.begin();
  if (it == indices.end()) {
    cout << "Illegal date" << endl;
  }
  else {
    while (it != indices.end()) {
      cout << *(it++) << endl;
    }
  }
}

int main()
{
  init_offsets();

  int perm[3];
  perm[0] = 0;
  perm[1] = 1;
  perm[2] = 2;

  perm_gen(perm, 0, 3);

  int n;
  cin >> n;

  for (int i = 1; i <= n; ++i) {
    printf("Scenario #%d:\n", i);

    solve_case();

    cout << endl;
  }
}
