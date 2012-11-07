#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <iostream>

using namespace std;

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

void solve_case(int *I, int *E, double *A)
{
  int n;
  cin >> n;

  *I = 0;
  *E = 0;
  *A = 0;

  int x = 0;
  int y = 0;
  int area2 = 0;
  for (int i = 0; i < n; ++i) {
    int dx, dy;
    cin >> dx >> dy;

    int pos_x = abs(dx);
    int pos_y = abs(dy);

    *E += pos_x == 0 || pos_y == 0 ? max(pos_x, pos_y) : gcd(pos_x, pos_y);
    area2 += (y + y + dy) * dx;

    x += dx;
    y += dy;
  }

  area2 = area2 < 0 ? -area2 : area2;
  *A = area2 / 2.0;
  *I = (area2 + 2 - *E) / 2;
}

int main()
{
  int n;

  cin >> n;
  for (int i = 1; i <= n; ++i) {
    int I, E;
    double A;

    solve_case(&I, &E, &A);

    printf("Scenario #%d:\n", i);
    printf("%d %d %.1lf\n\n", I, E, A);
  }
}
