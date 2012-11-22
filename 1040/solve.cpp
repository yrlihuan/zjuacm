#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <set>
#include <map>
#include <vector>
#include <list>
#include <iostream>

using namespace std;

#define IS_TO_FRONT(v) ((v)%6==3)
#define IS_AWAY_FRONT(v) ((v)%6==0)
#define IS_AWAY_BACK_NEG(v) ((v)%6==1)
#define IS_AWAY_BACK_POS(v) ((v)%6==2)

class Signal
{
public:
  string name;
  string connects[2];
  int vertex;
  int index;
};

class Point
{
public:
  string name;
  string connects[3]; // front, back -, back +
  int vertices[6]; // away front, back -, back +; to front ...
};

class Rule
{
public:
  string x;
  string y;
  string sign;
};

typedef map<string, Point>::iterator IterP;
typedef map<string, Signal>::iterator IterS;

void build_graph(int &n, map<int, set<int> > &edges, map<string, Point> &points, map<string, Signal> &signals)
{
  int pid = 0;
  int i;
  int j;

  // First setup graph vertice for points
  IterP it = points.begin();
  while (it != points.end()) {
    Point &p = it->second;
    for (i = 0; i < 6; ++i) {
      p.vertices[i] = pid++;
    }

    edges[p.vertices[3]].insert(p.vertices[1]);
    edges[p.vertices[3]].insert(p.vertices[2]);
    edges[p.vertices[4]].insert(p.vertices[0]);
    edges[p.vertices[5]].insert(p.vertices[0]);

    ++it;
  }

  // Then process points to extract connections
  it = points.begin();
  while (it != points.end()) {
    Point &p = it->second;

    for (i = 0; i < 3; ++i) {
      string conn = p.connects[i];
      IterP pit2  = points.find(conn);
      string name = p.name;
      int index = 0;
      while (pit2 == points.end()) {
        if (conn == "XXX") {
          break;
        }

        IterS sit = signals.find(conn);
        Signal &s = sit->second;
        for (j = 0; j < 2; ++j) {
          if (s.connects[j] == name) {
            name = s.name;
            conn = s.connects[1-j];

            if (s.vertex == -1) {
              s.vertex = p.vertices[3*j+i];
              s.index = index++;
            }

            break;
          }
        }

        pit2 = points.find(conn);
      }

      if (pit2 != points.end()) {
        Point &p2 = pit2->second;
        for (j = 0; j < 3; ++j) {
          if (p2.connects[j] == name) {
            edges[p.vertices[i]].insert(p2.vertices[j+3]);
            edges[p2.vertices[j]].insert(p.vertices[i+3]);
            break;
          }
        }

        continue;
      }
    }

    ++it;
  }

  // Process untouched signals
  IterS sit = signals.begin();
  while (sit != signals.end()) {
    Signal &s = sit->second;
    if (s.vertex != -1) {
      sit++;
      continue;
    }

    for (j = 0; j < 2; ++j) {
      if (s.connects[j] == "XXX") {
        int ids[2]; // away from "XXX", to "XXX"
        ids[0] = pid++;
        ids[1] = pid++;

        string from = "XXX";
        string name = s.name;
        int index = 0;

        while (name != "XXX") {
          IterS sit2 = signals.find(name);
          if (sit2 == signals.end()) {
            break;
          }

          Signal &s2 = sit2->second;
          for (int k = 0; k < 2; ++k) {
            if (s2.connects[k] == from) {
              s2.vertex = ids[k];

              if (k == 0) {
                s2.index = index++;
              }
              else {
                s2.index = index--;
              }

              from = s2.name;
              name = s2.connects[1-k];
              break;
            }
          }
        }

        break;
      }
    }

    sit++;
  }

  // DEBUG
  // it = points.begin();
  // while (it != points.end()) {
  //   Point &p = it->second;

  //   cout << p.name << endl;

  //   for (i = 0; i < 6; ++i) {
  //     int v = p.vertices[i];
  //     cout << v << ": ";
  //     set<int>::iterator ite = edges[v].begin();
  //     while (ite != edges[v].end()) {
  //       cout << *ite << "  ";
  //       ite++;
  //     }

  //     cout << endl;
  //   }

  //   cout << endl;

  //   ++it;
  // }

  // sit = signals.begin();
  // while (sit != signals.end()) {
  //   Signal &s = sit->second;

  //   cout << s.name << ": " << s.vertex << endl;
  //   ++sit;
  // }
}

bool search(
    int current,
    int target,
    list<int> &paths,
    map<int, set<int> > &edges)
{
  if (current == target) {
    return true;
  }

  set<int>::iterator it = edges[current].begin();
  set<int>::iterator end = edges[current].end();
  while (it != end) {
    paths.push_back(*it);

    bool found = search(*it, target, paths, edges);
    if (found) {
      return true;
    }

    paths.pop_back();
    ++it;
  }

  return false;
}

bool apply_rules(
    list<int> &paths,
    map<string, Signal> &signals,
    map<string, Point> &points,
    list<Rule> &rules,
    map<int, set<int> > &edges)
{
  vector<Signal> v_signals;
  for (map<string, Signal>::iterator it = signals.begin(); it != signals.end(); ++it) {
    v_signals.push_back(it->second);
  }

  vector<Point> v_points;
  for (map<string, Point>::iterator it = points.begin(); it != points.end(); ++it) {
    v_points.push_back(it->second);
  }

  int max_point_v = v_points.size() * 6;

  map<string, list<Rule> > rules_from;
  list<Rule>::iterator itr = rules.begin();
  while (itr != rules.end()) {
    rules_from[itr->x].push_back(*itr);
    ++itr;
  }

  list<int>::iterator itp = paths.begin();
  while (itp != paths.end()) {
    // cout << "checking x: " << *itp << endl;
    int v = *itp;
    if (v >= max_point_v || !IS_TO_FRONT(v)) {
      ++itp;
      continue;
    }

    Point x = v_points[v/6];

    Rule *valid_rule = NULL;
    list<int>::iterator target = paths.begin();
    list<Rule>::iterator itr2 = rules_from[x.name].begin();
    list<Rule>::iterator end = rules_from[x.name].end();

    while (itr2 != end) {
      string y = itr2->y;
      Point &py = points[y];
      int away_front = py.vertices[0];

      list<int>::iterator itp2 = itp;
      while (itp2 != paths.end()) {
        if (*itp2 == away_front) {
          valid_rule = &(*itr2);
          target = itp2;
          break;
        }

        ++itp2;
      }

      if (valid_rule) {
        break;
      }

      ++itr2;
    }

    int back = *(++itp);
    string choice = IS_AWAY_BACK_NEG(back) ? "-" : "+";
    --itp;

    bool need_switch = false;
    if (valid_rule) {
      if (valid_rule->sign != choice) {
        need_switch = true;
      }
    }
    else {
      if (choice != "-") {
        need_switch = true;
        target = paths.end();
        target--;
      }
    }

    if (need_switch) {
      int to = *target;
      int from = back + 1;
      if (IS_AWAY_BACK_POS(back)) {
        from = back - 1;
      }

      list<int> alt_paths;
      alt_paths.push_back(from);
      bool valid_alt = search(from, to, alt_paths, edges);
      if (valid_alt) {
        list<int>::iterator it_ins = paths.erase(++itp, ++target);
        paths.insert(it_ins, alt_paths.begin(), alt_paths.end());

        for (int k = 0; k < alt_paths.size(); ++k) {
          --it_ins;
        }

        itp = it_ins;
        itp--;
      }
    }

    ++itp;
  }
}

void print(string src, list<int> &paths, map<string, Point> &points, map<string, Signal> &signals)
{
  list<int>::iterator it = paths.begin();
  int max_point_v = points.size() * 6;

  map<int, list<Signal> > m_signals;
  IterS itms = signals.begin();
  while (itms != signals.end()) {
    Signal &s = itms->second;

    list<Signal>::iterator its = m_signals[s.vertex].begin();
    while (its != m_signals[s.vertex].end()) {
      if (its->index > s.index) {
        break;
      }

      ++its;
    }

    m_signals[s.vertex].insert(its, s);
    ++itms;
  }

  while (it != paths.end()) {
    int v = *it;
    list<Signal> sub_signals = m_signals[v];
    list<Signal>::iterator its = sub_signals.begin();

    if (v >= max_point_v) {
      if (its->name != src) {
        cout << "NOT POSSIBLE" << endl;
        return;
      }
    }

    while (its != sub_signals.end()) {
      cout << its->name << endl;
      ++its;
    }

    ++it;
  }

  vector<Point> v_points;
  for (map<string, Point>::iterator it = points.begin(); it != points.end(); ++it) {
    v_points.push_back(it->second);
  }

  it = paths.begin();
  while (it != paths.end()) {
    if (*it >= max_point_v || (*it) % 3 == 0) {
      ++it;
      continue;
    }

    int v = *it / 6;

    Point &p = v_points[v];
    cout << p.name;

    if (*it%3 == 1) {
      cout << " -" << endl;
    }
    else {
      cout << " +" << endl;
    }

    ++it;
  }
}

void solve_case(string src, string dest, map<string, Point> &points, map<string, Signal> &signals, list<Rule> &rules)
{
  map<int, set<int> > edges;
  int n;
  build_graph(n, edges, points, signals);

  int v_src = signals.find(src)->second.vertex;
  int v_dest = signals.find(dest)->second.vertex;

  list<int> paths;
  paths.push_back(v_src);
  bool path_found = search(v_src, v_dest, paths, edges);

  if (!path_found) {
    cout << "NOT POSSIBLE" << endl;
    return;
  }

  //list<int>::iterator it = paths.begin();
  //while (it != paths.end()) {
  //  cout << *it << " ";
  //  ++it;
  //}
  //cout << endl;

  apply_rules(paths, signals, points, rules, edges);

  //it = paths.begin();
  //while (it != paths.end()) {
  //  cout << *it << " ";
  //  ++it;
  //}
  //cout << endl;

  print(src, paths, points, signals);
}

int main()
{
  int cases;
  cin >> cases;

  for (int ind = 1; ind <= cases; ++ind) {
    string from;
    string to;
    cin >> from >> to;

    map<string, Signal> signals;
    map<string, Point> points;
    list<Rule> rules;

    int n;
    cin >> n;
    for (int i = 0; i < n; ++i) {
      string type;
      cin >> type;
      if (type == "S") {
        Signal s;
        s.vertex = -1;
        cin >> s.name >> s.connects[0] >> s.connects[1];

        signals[s.name] = s;
      }
      else {
        Point p;
        cin >> p.name >> p.connects[0] >> p.connects[1] >> p.connects[2];

        points[p.name] = p;
      }
    }

    cin >> n;
    for (int i = 0; i < n; ++i) {
      string type;
      cin >> type;

      Rule r;
      cin >> r.x >> r.y >> r.sign;

      rules.push_back(r);
    }

    // printf("%ld, %ld, %ld\n", signals.size(), points.size(), rules.size());
    printf("Scenario #%d:\n", ind);
    solve_case(from, to, points, signals, rules);
    cout << endl;
  }
}
