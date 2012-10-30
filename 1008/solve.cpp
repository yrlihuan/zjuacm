#include <stdio.h>
#include <stdlib.h>

int used[25];
int placed[5][5];

#define TOP 0
#define RIGHT 1
#define BOTTOM 2
#define LEFT 3

#define WILDCARD -1

int compare(const void *a, const void *b)
{
  return *((int*)a) - *((int*)b);
}

void sort_matrix(int squares[][4], int cnt)
{
  int hash[cnt];

  for (int i = 0; i < cnt; ++i) {
    hash[i] = (squares[i][TOP] << 24) + (squares[i][RIGHT] << 16) + (squares[i][BOTTOM] << 8) + squares[i][LEFT];
  }

  qsort(hash, cnt, sizeof(int), compare);
  for (int i = 0; i < cnt; ++i) {
    squares[i][TOP] = (hash[i] >> 24) & 0xff;
    squares[i][RIGHT] = (hash[i] >> 16) & 0xff;
    squares[i][BOTTOM] = (hash[i] >> 8) & 0xff;
    squares[i][LEFT] = hash[i] & 0xff;
  }
}

bool case_solve(int n)
{
  int cnt = n * n;
  int n2 = n * 2;
  int squares[cnt][4];

  for (int i = 0; i < cnt; ++i) {
    // top, right, bottom, left
    scanf("%d %d %d %d", squares[i]+TOP,squares[i]+RIGHT,squares[i]+BOTTOM,squares[i]+LEFT);
    used[i] = false;
  }

  //char arrx[] = {0, 1, 0, 1, 2, 2, 0, 1, 2, 3, 3, 3, 0, 1, 2, 3, 4, 4, 4, 4, 0, 1, 2, 3, 4};
  //char arry[] = {0, 0, 1, 1, 0, 1, 2, 2, 2, 0, 1, 2, 3, 3, 3, 3, 0, 1, 2, 3, 4, 4, 4, 4, 4};

  char arrx[cnt];
  char arry[cnt];
  for (int i = 0; i < cnt; ++i) {
    if (i < n2) {
      arrx[i] = i / 2;
      arry[i] = i % 2;
    }
    else {
      arrx[i] = i % n;
      arry[i] = i / n;
    }
  }

  sort_matrix(squares, cnt);
  //for (int i = 0; i < cnt; ++i) {
  //  printf("%d %d %d %d\n", squares[i][TOP],squares[i][RIGHT],squares[i][BOTTOM],squares[i][LEFT]);
  //}

  int pos = 0;
  int search_ind = 0;
  while (true) {
    if (pos == cnt) {
      return true;
    }

    int x = arrx[pos];
    int y = arry[pos];

    int top = y > 0 ? squares[placed[x][y-1]][BOTTOM] : WILDCARD;
    int left = x > 0 ? squares[placed[x-1][y]][RIGHT] : WILDCARD;

    bool match_found = false;
    for (int i = search_ind; i < cnt; ++i) {
      if (!used[i] &&
          (top == WILDCARD || top == squares[i][TOP]) &&
          (left == WILDCARD || left == squares[i][LEFT])) {
        used[i] = true;
        placed[x][y] = i;

        match_found = true;
        break;
      }
    }

    if (match_found) {
      //int i = placed[x][y];
      //printf("%d %d: %d %d %d %d\n", x, y, squares[i][TOP],squares[i][RIGHT],squares[i][BOTTOM],squares[i][LEFT]);
      pos += 1;
      search_ind = 0;
    }
    else {
      if (pos == 0) {
        return false;
      }

      pos -= 1;

      int x = arrx[pos];
      int y = arry[pos];

      int i = placed[x][y];
      used[i] = false;

      search_ind = i + 1;
      while (search_ind < cnt) {
        int j = search_ind;
        if (squares[j][TOP] != squares[i][TOP] ||
            squares[j][RIGHT] != squares[i][RIGHT] ||
            squares[j][BOTTOM] != squares[i][BOTTOM] ||
            squares[j][LEFT] != squares[i][LEFT]) {
          break;
        }

        search_ind++;
      }
    }
  }
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

    if (case_solve(n)) {
      printf("Game %d: Possible\n", cnt);
    }
    else {
      printf("Game %d: Impossible\n", cnt);
    }
  }
}


