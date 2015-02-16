#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>

using namespace std;

int ForbiddenSlots(int state) {
  int forbidden = state;
  for (int i = 2; i <= 20; ++i) {
    for (int j = i + 1; j <= 20; ++j) {
      int m1 = 1 << i;
      int m2 = 1 << j;

      if (m1 & state) {
        for (int a = 1; a * i <= 20; ++a) {
          forbidden |= 1 << (a * i);
        }
      }

      if (((m1 & state)) && ((m2 & state))) {
        for (int a = 1; a * i <= 20; ++a) {
          for (int b = 1; a * i + b * j <= 20; ++b) {
            forbidden |= 1 << (a * i + b * j);
          }
        }
      }
    }
  }

  static int output_cnt = 0;
  if (output_cnt-- > 0) {
    printf("S: %x, F: %x\n", state, forbidden);
  }

  return forbidden;
}

void NextStates(int state, vector<int>* next) {
  int forbidden = ForbiddenSlots(state);
  for (int i = 2; i <= 20; ++i) {
    if (!((1 << i) & forbidden)) {
      next->push_back(state | (1 << i));
    }
  }
}

// value is true: winning position.
// value is false: losing position.
map<int, bool> known_positions;

bool IsWinningPosition(int state);
bool IsLosingPosition(int state) {
  if (known_positions.find(state) != known_positions.end()) {
    return !known_positions[state];
  }

  int forbidden = ForbiddenSlots(state);
  if (forbidden == 0x1ffffc) {
    known_positions[state] = false;
    return true;
  }

  vector<int> next;
  NextStates(state, &next);
  bool next_wins = true;
  for (int k = 0; k < next.size(); ++k) {
    int s = next[k];
    if (!IsWinningPosition(s)) {
      next_wins = false;
    }
  }

  known_positions[state] = !next_wins;
  return next_wins;
}

void GetWinningMoves(int state, vector<int>* moves) {
  vector<int> next;
  NextStates(state, &next);
  for (int k = 0; k < next.size(); ++k) {
    int s = next[k];
    if (IsLosingPosition(s)) {
      int m = s ^ state;
      for (int i = 2; i <= 20; ++i) {
        if (m & (1 << i)) {
          moves->push_back(i);
          break;
        }
      }
    }
  }
}

bool IsWinningPosition(int state) {
  if (known_positions.find(state) != known_positions.end()) {
    return known_positions[state];
  }

  int forbidden = ForbiddenSlots(state);
  if (forbidden == 0x1ffffc) {
    known_positions[state] = false;
    return false;
  }

  vector<int> next;
  NextStates(state, &next);
  bool next_loses = false;
  for (int k = 0; k < next.size(); ++k) {
    int s = next[k];
    if (IsLosingPosition(s)) {
      next_loses = true;
    }
  }

  known_positions[state] = next_loses;
  return next_loses;
}

int main()
{
  int n;

  cin >> n;
  for (int i = 1; i <= n; ++i) {
    printf("Scenario #%d:\n", i);

    int cnt;
    cin >> cnt;

    int state = 0x1ffffc;
    for (int j = 0; j < cnt; ++j) {
      int p;
      cin >> p;
      state &= ~(1 << p);
    }

    vector<int> moves;
    if (IsWinningPosition(state)) {
      GetWinningMoves(state, &moves);
    }

    if (moves.size() > 0) {
      cout << "The winning moves are:";
      for (int j = 0; j < moves.size(); ++j) {
        cout << " " << moves[j];
      }
      cout << "." << endl;
    } else {
      cout << "There is no winning move." << endl;
    }

    cout << endl;
  }
}
