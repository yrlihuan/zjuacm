#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <vector>

#define MAX_C 512

using namespace std;

int index(int c0, int c1) {
  return MAX_C * c0 + c1;
}

int case_solve(int n)
{
  int w[3];
  int s[3];
  int d[3];

  int set[3];
  int set_d;

  int weight[n];
  int size[n];

  int i;

  for (i = 0; i < 3; ++i) {
    scanf("%d %d %d", w+i, s+i, d+i);
  }

  scanf("%d %d %d %d", set, set+1, set+2, &set_d);

  for (i = 0; i < n; ++i) {
    scanf("%d %d", weight+i, size+i);
  }

  int cache_size = MAX_C * MAX_C;
  short *cache1 = new short[cache_size];
  short *cache2 = new short[cache_size];
  memset(cache1, 0xff, sizeof(short)*cache_size);

  cache1[0] = 0;

  // test data proves that 100k values are possible
  int *valid_positions = new int[100000];
  for (i = 0; i < n; ++i) {
    memcpy(cache2, cache1, sizeof(short)*cache_size);

    int shipw = weight[i];
    int ships = size[i];

    int max0 = min(shipw / w[0], ships / s[0]);
    int max1 = min(shipw / w[1], ships / s[1]);

    // printf("%d %d\n", max0, max1);

    int a0, a1, a2;
    int first_path = true;
    int p = 0;
    for (a0 = 0; a0 <= max0; ++a0) {
      int wr0 = shipw - a0 * w[0];
      int sr0 = ships - a0 * s[0];

      for (a1 = 0; a1 <= max1; ++a1) {
        int wr1 = wr0 - a1 * w[1];
        int ws1 = sr0 - a1 * s[1];

        if (wr1 < 0 || ws1 < 0) {
          break;
        }

        int ind_delta = index(a0, a1);
        a2 = min(wr1 / w[2], ws1 / s[2]);

        if (first_path) {
          first_path = false;
          for (int j = 0; j < cache_size; ++j) {
            if (cache1[j] != -1) {
              valid_positions[p++] = j;
              int old = cache2[j+ind_delta];
              int value = cache1[j] + a2;
              if (value > old) {
                cache2[j+ind_delta] = value;
              }
            }
          }
        }
        else {
          int old_p = p;
          for (int s = 0; s < old_p; ++s) {
            int j = valid_positions[s];
            int old = cache2[j+ind_delta];
            int value = cache1[j] + a2;
            if (value > old) {
              cache2[j+ind_delta] = value;
            }
          }
        }
      }
    }

    /*
    for (int j = 0; j < cache_size; ++j) {
      int a0 = j / MAX_C;
      int a1 = j % MAX_C;
      int a2 = cache2[j];

      if (a2 >= 0) {
        printf("%d %d %d\n", a0, a1, a2);
      }
    }
    */

    short *tmp = cache1;
    cache1 = cache2;
    cache2 = tmp;
  }

  int max_d = 0;
  for (i = 0; i < cache_size; ++i) {
    int a0 = i / MAX_C;
    int a1 = i % MAX_C;
    int a2 = cache1[i];
    if (a2 < 0) {
      continue;
    }

    int defense = 0;
    while (a0 >= set[0] && a1 >= set[1] && a2 >= set[2]) {
      a0 -= set[0];
      a1 -= set[1];
      a2 -= set[2];

      defense += set_d;
    }

    defense += a0 * d[0] + a1 * d[1] + a2 * d[2];

    max_d = max(max_d, defense);
  }

  delete []cache1;
  delete []cache2;
  delete []valid_positions;

  return max_d;
}

int main()
{
  int n;
  int cnt = 0;
  while (scanf("%d", &n) != EOF) {
    if (!n)
      break;

    ++cnt;
    if (cnt != 1) {
      printf("\n");
    }

    printf("Case %d: %d\n", cnt, case_solve(n));
  }
}


