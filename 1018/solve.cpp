#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <algorithm>
#include <vector>

using namespace std;

#define MAX_SECTIONS 256

// This is very important.
// 0.0000001 and 0.000000001 will not pass the test
#define ACCURACY 0.00000001

// for poj
#ifndef M_PI
#define M_PI 3.1415926535897932384
#endif

typedef struct PointType
{
  double x;
  double y;
} Point;

int relative_position(Point p, const vector<Point> &hill)
{
  int size = hill.size();

  int i;
  for (i = 1; i < size; ++i) {
    if (hill[i].x <= p.x)
      break;
  }

  double x0 = hill[i].x;
  double y0 = hill[i].y;
  double x1 = hill[i-1].x;
  double y1 = hill[i-1].y;

  double hill_y = y1 - (y1-y0)*(x1-p.x)/(x1-x0);

  if (fabsl(hill_y-p.y) < ACCURACY) {
    return 0;
  }
  else {
    return p.y - hill_y < 0 ? -1 : 1;
  }
}

Point rotate_around(Point p, Point center, double angle)
{
  double dx = p.x - center.x;
  double dy = p.y - center.y;
  double l = sqrtl(dx * dx + dy * dy);

  double sin0 = dy / l;
  double cos0 = dx / l;

  double sin1 = cosl(angle)*sin0 + sinl(angle)*cos0;
  double cos1 = cosl(angle)*cos0 - sinl(angle)*sin0;

  dy = sin1 * l;
  dx = cos1 * l;

  // printf("dx dy: %lf %lf\n", dx, dy);

  p.x = center.x + dx;
  p.y = center.y + dy;

  return p;
}

bool try_rotate(Point *center, Point *vertices, int n, const vector<Point> hill)
{
  Point rotatable;
  int rotatable_index = -1;
  double initial_sign = 0.0;
  int i;
  for (i = 0; i < n; ++i) {
    if (0 == relative_position(vertices[i], hill)) {
      if (rotatable_index == -1) {
        rotatable = vertices[i];
        rotatable_index = i;
        initial_sign =  rotatable.x < center->x ? -1.0 : 1.0;
      }
      else {
        Point v0 = rotatable;
        Point v1 = vertices[i];

        // If there are two contacting points,
        // these two points must be at the same
        // side (in x axis) of the center
        if ((v0.x-center->x) * (v1.x-center->x) > 0) {
          if (fabsl(v1.x-center->x) < fabsl(v0.x-center->x)) {
            rotatable = v1;
            rotatable_index = i;
            initial_sign =  rotatable.x < center->x ? -1.0 : 1.0;
          }
        }
        else {
          return false;
        }
      }
    }
  }

  if (rotatable_index == -1) {
     // printf("no valid vertex found!\n");
     while (true) {
       ;
     }
  }

  // printf("rotating center: %lf, %lf\n", rotatable.x, rotatable.y);

  double angle = 0.0 / M_PI;
  double step = 1.0 / M_PI / 180.0;
  bool decreasing_steps = false;
  bool new_contact_vertex = false;

  while (!new_contact_vertex) {
    angle += step * initial_sign;
    // printf("angle: %lf\n", angle);

    bool over_rotated = false;
    for (i = 0; i < n; ++i) {
      if (i == rotatable_index)
        continue;

      Point p = rotate_around(vertices[i], rotatable, angle);
      // printf("%lf %lf\n", p.x, p.y);

      int comp = relative_position(p, hill);
      if (comp == 0) {
        new_contact_vertex = true;
      }
      else if (comp < 0) {
        // printf("over rotated\n");
        over_rotated = true;
      }
    }

    if (!over_rotated && new_contact_vertex) {
      continue;
    }
    else if (over_rotated) {
      new_contact_vertex = false;
      decreasing_steps = true;
      step = -fabsl(step/2);
    }
    else {
      if (decreasing_steps) {
        step = fabsl(step/2);
      }
      else {
        step = fabsl(step);
      }
    }
  }

  for (i = 0; i < n; ++i) {
    if (i == rotatable_index)
      continue;

    vertices[i] = rotate_around(vertices[i], rotatable, angle);
  }

  Point p = rotate_around(*center, rotatable, angle);
  center->x = p.x;
  center->y = p.y;

  // printf("index: %d\n", rotatable_index);
  // printf("center: %lf %lf\n", center->x, center->y);
  // printf("angle: %lf\n", angle);

  return true;
}

void calc_hill_articulation_points(
    vector<Point> &hill,
    Point start,
    const vector<double> &lengths,
    const vector<double> &slopes)
{
  hill.push_back(start);

  int cnt = lengths.size();
  for (int i = 0; i < cnt; ++i) {
    Point p;
    double len = lengths[i];
    double slope = slopes[i];
    double angle = atanl(slope);
    p.x = start.x - cosl(angle) * len;
    p.y = start.y - sinl(angle) * len;

    hill.push_back(p);

    start = p;
  }
}

void solve_case()
{
  int i;
  int n;

  scanf("%d", &n);

  Point vertices[n];
  Point center;
  vector<Point> hill;

  for (i = 0; i < n; ++i) {
    scanf("%lf %lf", &(vertices[i].x), &(vertices[i].y));
  }

  scanf("%lf %lf", &(center.x), &(center.y));

  Point line_start;
  vector<double> slopes;
  vector<double> lengths;
  bool finished = false;

  while (!finished) {
    double slope;
    double length;

    scanf("%lf %lf", &length, &slope);
    slopes.push_back(slope);
    lengths.push_back(length);

    finished = slope == 0.0;
  }

  scanf("%lf %lf", &(line_start.x), &(line_start.y));

  calc_hill_articulation_points(hill, line_start, lengths, slopes);

  while (try_rotate(&center, vertices, n, hill))
    ;
  //try_rotate(&center, vertices, n, hill);

  Point p = center;
  printf("%.3lf %.3lf\n", p.x, p.y);
}

int main()
{
  int cnt = 0;
  scanf("%d", &cnt);

  for (int i = 0; i < cnt; ++i) {
    solve_case();
  }
}

