#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <set>
#include <map>
#include <vector>
#include <list>
#include <iostream>

using namespace std;

enum {
  ADD = 92834,
  SUB = 92741,
  MOV_VAR = 82931
};

enum {
  S_LVALUE = 0,
  S_OPRAND1,
  S_OP,
  S_OPRAND2,
  S_END
};

class Instruction
{
public:
  string op1;
  string op2;
  int type;

  Instruction();
};

Instruction::Instruction():
  op1(""), op2(""), type(MOV_VAR)
{
}

#define MAXN 101

map<string, double> states[MAXN][MAXN];
double weights[MAXN][MAXN];

void parse_programe(vector<Instruction> &result, map<string, double> &vars, int id)
{
  char buf[1024];

  sprintf(buf, "%d__r1", id);
  string str_r1(buf);

  sprintf(buf, "%d__r2", id);
  string str_r2(buf);

  vars[str_r1] = 0.0;
  vars[str_r2] = 0.0;

  fgets(buf, sizeof(buf), stdin);
  while (strcmp(buf, "END\n")) {
    // cout << buf << endl;

    int p = 0;
    int s = S_LVALUE;

    string lvalue;
    string oprand1;
    string oprand2;
    char op;

    while (s != S_END) {
      while (buf[p] == ' ' || buf[p] == ':' || buf[p] == '=')
        ++p;

      char var[128] = {0};
      char *c = var;

      if (s == S_OP) {
        op = buf[p++];
      }
      else {
        while ((buf[p] >= 'a' && buf[p] <= 'z') ||
              (buf[p] >= 'A' && buf[p] <= 'Z') ||
              (buf[p] >= '0' && buf[p] <= '9')) {
          if (buf[p] >= 'A' && buf[p] <= 'Z') {
            buf[p] += 32;
          }

          *(c++) = buf[p++];
        }

        if (s == S_LVALUE) {
          lvalue = var;
        }
        else if (s == S_OPRAND1) {
          oprand1 = var;
        }
        else if (s == S_OPRAND2) {
          oprand2 = var;
        }
      }

      s += 1;
    }

    // cout << lvalue << " := " << oprand1 << op << oprand2 << endl;

    Instruction move1;
    Instruction move2;
    Instruction calc;
    Instruction move3;

    move1.op1 = str_r1;
    if (oprand1[0] > '9') {
      move1.op2 = oprand1;
      move1.type = MOV_VAR;
      vars[oprand1] = 0.0;
    }
    else {
      move1.type = atoi(oprand1.c_str());
    }

    move2.op1 = str_r2;
    if (oprand2[0] > '9') {
      move2.op2 = oprand2;
      move2.type = MOV_VAR;
      vars[oprand2] = 0.0;
    }
    else {
      move2.type = atoi(oprand2.c_str());
    }

    calc.op1 = str_r1;
    calc.op2 = str_r2;
    if (op == '+') {
      calc.type = ADD;
    }
    else {
      calc.type = SUB;
    }

    move3.op1 = lvalue;
    move3.op2 = str_r1;
    move3.type = MOV_VAR;
    vars[lvalue] = 0.0;

    result.push_back(move1);
    result.push_back(move2);
    result.push_back(calc);
    result.push_back(move3);

    fgets(buf, sizeof(buf), stdin);
  }
}

void execute(Instruction inst, map<string, double> &vars, double weight)
{
  if (inst.type == MOV_VAR) {
    vars[inst.op1] = vars[inst.op2];
  }
  else if (inst.type == ADD) {
    vars[inst.op1] += vars[inst.op2];
  }
  else if (inst.type == SUB) {
    vars[inst.op1] -= vars[inst.op2];
  }
  else {
    vars[inst.op1] = inst.type * weight;
  }
}

void solve(int x, int y, const vector<Instruction> &inst1, const vector<Instruction> &inst2)
{
  if (weights[x][y]) {
    return;
  }

  int m = inst1.size() + 1;
  int n = inst2.size() + 1;

  map<string, double> cache1 = states[0][0];
  double w1 = 0.0;
  if (x - 1 >= 0) {
    solve(x-1, y, inst1, inst2);

    cache1 = states[x-1][y];
    w1 = weights[x-1][y];
    if (y == n - 1) {
      w1 += w1;
      map<string, double>::iterator it = cache1.begin();
      while (it != cache1.end()) {
        (*it).second += (*it).second;
        it++;
      }
    }

    execute(inst1[x-1], cache1, w1);
  }

  map<string, double> cache2 = states[0][0];
  double w2 = 0.0;
  if (y - 1 >= 0) {
    solve(x, y-1, inst1, inst2);
    cache2 = states[x][y-1];
    w2 = weights[x][y-1];
    if (x == m - 1) {
      w2 += w2;
      map<string, double>::iterator it = cache2.begin();
      while (it != cache2.end()) {
        (*it).second += (*it).second;
        it++;
      }
    }

    execute(inst2[y-1], cache2, w2);
  }

  // printf("%d, %d\n", x, y);
  weights[x][y] = w1 + w2;
  map<string, double>::iterator it = cache1.begin();
  while (it != cache1.end()) {
    states[x][y][it->first] = cache1[it->first] + cache2[it->first];
    // printf("%s: %lf, %lf\n", it->first.c_str(), states[x][y][it->first], weights[x][y]);

    ++it;
  }

}

int main()
{
  int cases;

  cin >> cases;
  while (cases--)
  {
    map<string, double> vars;

    vector<Instruction> instructions1;
    vector<Instruction> instructions2;

    char buf[128];
    fgets(buf, 128, stdin); // skipping the first empty line
    parse_programe(instructions1, vars, 0);
    parse_programe(instructions2, vars, 1);

    weights[0][0] = 1.0;
    int m = instructions1.size() + 1;
    int n = instructions2.size() + 1;

    for (int i = 0; i < m; ++i) {
      for (int j = 0; j < n; ++j) {
        states[i][j] = vars;
      }
    }

    solve(m-1, n-1, instructions1, instructions2);
    double sum = 0.0;
    double w = weights[m-1][n-1];
    map<string, double>::iterator it = states[m-1][n-1].begin();
    while (it != states[m-1][n-1].end()) {
      if (it->first[0] > '9') {
        printf("%.4lf\n", it->second / w);
      }

      ++it;
    }

    // cout << w << endl;

    for (int i = 0; i < m; ++i) {
      for (int j = 0; j < n; ++j) {
        states[i][j].clear();
      }
    }

    memset(weights, 0, sizeof(weights));

    if (cases)
      cout << endl;
  }
}
