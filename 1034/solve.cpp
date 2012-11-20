#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <set>
#include <map>
#include <vector>
#include <list>
#include <iostream>

#define MAXN 25
#define NPRIMES 8

using namespace std;

typedef struct RatioType
{
  int upper;
  int lower;
} Ratio;

void print(int coef[][MAXN], int n) {
  for (int s = 0; s < NPRIMES; ++s) {
    for (int t = 0; t < n; ++t) {
      printf("%4d", coef[s][t]);
    }

    cout << endl;
  }

  cout << endl;
}

int compare(const void *coef, const void *b)
{
  return *(int*)coef - *(int*)b;
}

int gcd(int coef, int b)
{
  while (b) {
    int c = b;
    b = coef % b;
    coef = c;
  }

  return coef;
}

int lcm(int coef, int b)
{
  return coef * b / gcd(coef,b);
}

bool initialize_equ(int coef[][MAXN], int c[], int n, Ratio r)
{
  int primes[] = {2, 3, 5, 7, 11, 13, 17, 19};

  memset(coef, 0, sizeof(int)*MAXN*NPRIMES);

  int e;
  for (int i = 1; i < n; ++i) {
    e = c[i];
    for (int j = 0; j < NPRIMES; ++j) {
      int p = primes[j];
      while (e % p == 0) {
        coef[j][i-1]++;
        e /= p;
      }
    }
  }

  int GCD = gcd(r.upper, r.lower);
  r.upper /= GCD;
  r.lower /= GCD;

  e = r.upper;
  for (int j = 0; j < NPRIMES; ++j) {
    int p = primes[j];
    while (e % p == 0) {
      coef[j][n-1]--;
      e /= p;
    }
  }

  if (e != 1) {
    return false;
  }

  e = r.lower;
  for (int j = 0; j < NPRIMES; ++j) {
    int p = primes[j];
    while (e % p == 0) {
      coef[j][n-1]++;
      e /= p;
    }
  }

  if (e != 1) {
    return false;
  }

  return true;
}

bool solve_square(int coef[][MAXN], int k, int n)
{
  int temp;
  int var = k;
  int i;
  int j;
  int x[k];
  memset(x, 0, sizeof(x));
  for (i = var - 1; i >= 0; i--)
  {
      temp = coef[i][n-1];
      for (j = i + 1; j < var; j++)
      {
          if (coef[i][j] != 0)
            temp -= coef[i][j] * x[j];
      }

      if (temp % coef[i][i] != 0)
        return false; // 说明有浮点数解，但无整数解.

      x[i] = temp / coef[i][i];
  }

  return true;
}

bool solve_equation(int coef[][MAXN], int n)
{
  int i, j, k;
  int max_r; // 当前这列绝对值最大的行.
  int col; // 当前处理的列.
  int ta, tb;
  int LCM;
  int temp;
  int free_x_num;
  int free_index;
  int var = n - 1;
  int equ = NPRIMES;

  bool free_x[MAXN];
  int free_num = 0;
  int x[MAXN] = {0};

  memset(free_x, 1, sizeof(free_x));

  // 转换为阶梯阵.
  col = 0; // 当前处理的列.
  int free_col = 0;
  for (k = 0; k < equ && col < var - free_col; k++, col++)
  {
    // 枚举当前处理的行.
    // 找到该col列元素绝对值最大的那行与第k行交换.(为了在除法时减小误差)
    max_r = k;
    for (i = k + 1; i < equ; i++) {
      if (abs(coef[i][col]) > abs(coef[max_r][col])) {
        max_r = i;
      }
    }

    if (max_r != k) {
      // 与第k行交换.
      for (j = k; j < var + 1; j++)
        swap(coef[k][j], coef[max_r][j]);
    }

    if (coef[k][col] == 0) {
      // 说明该col列第k行以下全是0了，则处理当前行的下一列.
      free_col++;
      for (i = 0; i < equ; ++i) {
        swap(coef[i][col], coef[i][var-free_col]);
      }

      k--;
      col--;
      continue;
    }

    for (i = k + 1; i < equ; i++) {
      // 枚举要删去的行.
      if (coef[i][col] != 0)
      {
        LCM = lcm(abs(coef[i][col]), abs(coef[k][col]));
        ta = LCM / abs(coef[i][col]), tb = LCM / abs(coef[k][col]);
        if (coef[i][col] * coef[k][col] < 0)
          tb = -tb; // 异号的情况是两个数相加.

        for (j = col; j < var + 1; j++) {
          coef[i][j] = coef[i][j] * ta - coef[k][j] * tb;
        }
      }
    }
  }

  //print(coef, n);
  //printf("row: %d of %d\n", k, equ);
  //printf("col: %d of %d\n", col, var);

  // 1. 无解的情况: 化简的增广阵中存在(0, 0, ..., coef)这样的行(coef != 0).
  for (i = k; i < equ; i++)
  { // 对于无穷解来说，如果要判断哪些是自由变元，那么初等行变换中的交换就会影响，则要记录交换.
    if (coef[i][var] != 0)
      return false;
  }

  //cout << "cp 1\n";
  // 2. 无穷解的情况: 在var * (var + 1)的增广阵中出现(0, 0, ..., 0)这样的行，即说明没有形成严格的上三角阵.
  // 且出现的行数即为自由变元的个数.
  if (k < var)
  {
    int free_cnt = var - k;
    int iters = 1 << (3 * free_cnt);

    if (free_cnt > 7) {
      return true;
    }

    for (i = 0; i < iters; ++i) {
      for (j = 0; j < free_cnt; ++j) {
        int value = (i >> (j*3)) & 0x7;
        value -= 4;

        for (int s = 0; s < k; ++s) {
          coef[s][var] += value * coef[s][k+j];
        }
      }

      bool valid = solve_square(coef, k, n);
      if (valid)
        return true;

      for (j = 0; j < free_cnt; ++j) {
        int value = (i >> (j*3)) & 0x7;
        value -= 4;

        for (int s = 0; s < k; ++s) {
          coef[s][var] -= value * coef[s][k+j];
        }
      }
    }

    return false;
  }

  //cout << "cp 2\n";

  // 3. 唯一解的情况: 在var * (var + 1)的增广阵中形成严格的上三角阵.
  // 计算出Xn-1, Xn-2 ... X0.
  return solve_square(coef, k, n);
}

void solve_case(int c[], int n, Ratio ratios[], int cnt)
{
  qsort(c, n, sizeof(int), compare);

  int i;
  for (int i = n-1; i >= 0; --i) {
    c[i] /= c[0];
  }

  for (i = 0; i < cnt; ++i) {
    int coef[8][MAXN];
    bool valid = initialize_equ(coef, c, n, ratios[i]);

    if (valid) {
      valid = solve_equation(coef, n);
    }

    if (valid) {
      printf("Gear ratio %d:%d can be realized.\n", ratios[i].upper, ratios[i].lower);
    }
    else {
      printf("Gear ratio %d:%d cannot be realized.\n", ratios[i].upper, ratios[i].lower);
    }
  }
}

int main()
{
  int cases = 0;
  cin >> cases;
  for (int ind = 1; ind <= cases; ++ind) {
    cout << "Scenario #" << ind << ":" << endl;

    int n;
    cin >> n;

    int c[n];
    for (int i = 0; i < n; ++i) {
      cin >> c[i];
    }

    int cnt;
    cin >> cnt;
    Ratio ratios[cnt];
    for (int i = 0; i < cnt; ++i) {
      cin >> ratios[i].upper >> ratios[i].lower;
    }

    solve_case(c, n, ratios, cnt);

    cout << endl;
  }
}
