#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <algorithm>
#include <vector>

// for poj
#ifndef M_PI
#define M_PI 3.1415926535897932384
#endif

#define ACCURACY 0.00001

typedef struct PointType
{
  double x;
  double y;
} Point;

double dist(Point a, Point b)
{
  double dx = a.x - b.x;
  double dy = a.y - b.y;
  return sqrt(dx * dx + dy * dy);
}

Point add(Point a, double dx, double dy)
{
  Point r;
  r.x = a.x + dx;
  r.y = a.y + dy;

  return r;
}

double solve_case(Point a, Point b, Point c)
{
  b = add(b, -a.x, -a.y);
  c = add(c, -a.x, -a.y);
  a = add(a, -a.x, -a.y);

  double bl = dist(b, a);
  double cx = c.x * b.x / bl + c.y * b.y / bl;
  double cy = c.y * b.x / bl - c.x * b.y / bl;

  b.x = bl;
  b.y = 0;
  c.x = cx;
  c.y = abs(cy);

  double x = bl / 2;
  double y = (cx * cx + cy * cy - cx * bl) / 2 / cy;

  return sqrt(x * x + y * y) * 2 * M_PI;
}

int main()
{
  Point a;
  Point b;
  Point c;

  while (scanf("%lf", &a.x) != EOF) {
    scanf("%lf", &a.y);
    scanf("%lf %lf", &b.x, &b.y);
    scanf("%lf %lf", &c.x, &c.y);

    double circumfence = solve_case(a, b, c);

    printf("%.2lf\n", circumfence);
  }
}


