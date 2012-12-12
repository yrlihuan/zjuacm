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

#define MAXN 200

class Point
{
public:
  Point(double x, double y) : x(x), y(y) {}
  Point() : x(0), y(0) {}
  double x;
  double y;
};

vector<int> connections[MAXN];
Point points[MAXN];
int N;

bool used[MAXN];
list<int> path;
int cnt;

Point p1(-1100.3, -1.1);
Point p2(1100.3, -1.3);
Point p3(1100.3, 1097.3);

Point pivots[3] = {p1, p2, p3};

// Outer product of Oa, Ob (O is point(0,0))
double outer_product(const Point &a, const Point &b)
{
  return a.x * b.y - b.x * a.y;
}

bool is_crossed(const Point &a1, const Point &a2,
                const Point &b1, const Point &b2)
{
  // Test 1
  if (min(a1.x, a2.x) > max(b1.x, b2.x) ||
      min(b1.x, b2.x) > max(a1.x, a2.x) ||
      min(a1.y, a2.y) > max(b1.y, b2.y) ||
      min(b1.y, b2.y) > max(a1.y, a2.y)
     )
  {
    return false;
  }

  // Test 2
  double r1 = outer_product(Point(b1.x-a1.x, b1.y-a1.y), Point(a2.x-a1.x, a2.y-a1.y));
  double r2 = outer_product(Point(b2.x-a1.x, b2.y-a1.y), Point(a2.x-a1.x, a2.y-a1.y));
  if (r1 * r2 >= 0) {
    return false;
  }

  r1 = outer_product(Point(a1.x-b1.x, a1.y-b1.y), Point(b2.x-b1.x, b2.y-b1.y));
  r2 = outer_product(Point(a2.x-b1.x, a2.y-b1.y), Point(b2.x-b1.x, b2.y-b1.y));

  return r1 * r2 < 0;
}

bool is_inside_polygon(const Point &p, const vector<Point> &vertices)
{
  int likelihood = 0;
  for (int i = 0; i < 3; ++i)
  {
    const Point &p2 = pivots[i];

    int cross = 0;
    int n = vertices.size();
    for (int i = 0; i < n; ++i) {
      Point a1 = vertices[i];
      Point a2 = vertices[(i+1)%n];

      bool both_crossed = is_crossed(a1, a2, p, p2);
      cross += both_crossed ? 1 : 0;
      //if (both_crossed) {
      //  cout << "crossed: " << i << endl;
      //}
    }

    //cout << p.x << " " << p.y << " cross: " << cross << endl;
    likelihood += cross % 2;
  }

  return likelihood > 1;
}

void when_circle_found(int n)
{
  vector<Point> vertices;

  list<int>::iterator it = path.begin();
  while (it != path.end()) {
    vertices.push_back(points[*it]);
    ++it;
  }

  //cout << "Polygon Found:" << endl;
  //vector<Point>::iterator pit = vertices.begin();
  //while (pit != vertices.end()) {
  //  cout << pit->x << " " << pit->y << endl;
  //  ++pit;
  //}

  bool valid_polygon = true;
  for (int i = 0; i < n; ++i) {
    if (used[i]) {
      continue;
    }

    Point p = points[i];
    if (is_inside_polygon(p, vertices)) {
      valid_polygon = false;
      break;
    }
  }

  //cout << "valid? " << valid_polygon << endl << endl;

  if (!valid_polygon) {
    return;
  }

  // check against edges
  for (int i = 0; i < N; ++i) {
    vector<int>::iterator it = connections[i].begin();
    while (it != connections[i].end()) {
      int a = i;
      int b = *it;

      if (used[a] && used[b]) {
        list<int>::iterator lit = find(path.begin(), path.end(), a);
        ++lit;
        if ((lit == path.end() && path.front() == b) || *lit == b) {
          ++it;
          continue;
        }

        --lit;

        if ((lit == path.begin() && path.back() == b) || *(--lit) == b) {
          ++it;
          continue;
        }
      }

      const Point &pa = points[a];
      const Point &pb = points[b];

      Point mid(pa.x/2+pb.x/2, pa.y/2+pb.y/2);
      if (is_inside_polygon(mid, vertices)) {
        valid_polygon = false;
        break;
      }

      ++it;
    }
  }

  cnt += valid_polygon ? 1 : 0;
}

void search(int root, int d, int n)
{
  int l = path.size();
  int last = path.back();
  vector<int> &conn = connections[last];

  if (l == d) {
    if (find(conn.begin(), conn.end(), root) != conn.end()) {
      when_circle_found(n);
    }

    return;
  }

  vector<int>::iterator it = conn.begin();
  while (it != conn.end()) {
    int c = *it;
    if (c > root && !used[c]) {
      path.push_back(c);
      used[c] = true;

      search(root, d, n);

      path.pop_back();
      used[c] = false;
    }

    ++it;
  }
}

int solve_case(int n, int d)
{
  path.clear();
  memset(used, 0, sizeof(used));
  cnt = 0;

  for (int i = 0; i < n; ++i) {
    path.push_back(i);
    used[i] = true;

    search(i, d, n);

    path.pop_back();
    used[i] = false;
  }

  return cnt / 2;
}

int main()
{
  int m;
  cin >> m;

  while (m--)
  {
    int n;
    cin >> n;
    N = n;

    memset(points, 0, sizeof(points));
    for (int i = 0; i < n; ++i)
    {
      connections[i].clear();
    }

    map<int,int> mapping;

    int ind = 0;

    for (int i = 0; i < n; ++i)
    {
      int p, x, y, d;
      cin >> p >> x >> y >> d;

      if (mapping.find(p) == mapping.end()) {
        mapping[p] = ind++;
      }

      p = mapping[p];
      points[p].x = x;
      points[p].y = y;

      while (d--) {
        int c;
        cin >> c;

        if (mapping.find(c) == mapping.end()) {
          mapping[c] = ind++;
        }

        c = mapping[c];

        connections[p].push_back(c);
      }
    }

    //map<int, int>::iterator it = mapping.begin();
    //while (it != mapping.end()) {
    //  cout << (it->first) << "=>" << (it->second) << endl;
    //  ++it;
    //}

    //int cnt = 0;
    //for (int i = 0; i < MAXN; ++i) {
    //  for (int j = 0; j < (int)connections[i].size(); ++j) {
    //    cnt += 1;
    //    int p = i;
    //    int c = connections[i][j];

    //    if (find(connections[c].begin(), connections[c].end(), p) == connections[c].end()) {
    //      cout << p << " " << c << endl;
    //    }
    //  }
    //}

    int k;
    cin >> k;

    if (k < 3) {
      cout << 0 << endl;
    }
    else {
      cout << solve_case(n, k) << endl;
    }
  }
}
