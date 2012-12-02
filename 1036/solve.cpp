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

void process_unknowns(char *unknowns[], char pi[][27], char pir[], char perm[], char rotates[], char *plain,
    set<int> &unknown_pis, bool &unknown_pir, bool &unknown_perm, bool &unknown_rotates)
{
  int ind = 0;

  int i, j;
  for (i = 0; i < 3; ++i) {
    for (j = 0; j < 26; ++j) {
      if (pi[i][j] == '?') {
        unknowns[ind++] = pi[i] + j;
        unknown_pis.insert(i);
      }
    }
  }

  char *v[] = {pir, perm, rotates};
  bool *b[] = {&unknown_pir, &unknown_perm, &unknown_rotates};

  for (i = 0; i < 3; ++i) {
    j = 0;
    char *p = v[i];
    while (*p != '\0') {
      if (*p == '?') {
        unknowns[ind++] = p;
        *b[i] = true;
      }

      p++;
      j++;
    }
  }

  while (*plain != '\0') {
    if (*plain == '?') {
      unknowns[ind++] = plain;
    }

    plain++;
  }

  while (ind < 3) {
    unknowns[ind++] = NULL;
  }
}

bool validate_settings(char pi[][27], char pir[], char perm[],
    const set<int> &unknown_pis, bool unknown_pir, bool unknown_perm)
{
  char *pp[5];
  pp[0] = pi[0];
  pp[1] = pi[1];
  pp[2] = pi[2];
  pp[3] = pir;
  pp[4] = perm;

  set<int> to_be_checked = unknown_pis;
  if (unknown_pir) {
    to_be_checked.insert(3);
  }

  if (unknown_perm) {
    to_be_checked.insert(4);
  }

  set<int>::iterator it = to_be_checked.begin();
  while (it != to_be_checked.end()) {
    int cnt = 0;
    char *p = pp[*it];

    while (*p != '\0') {
      cnt += *p - 'a';
      ++p;
    }

    if (cnt != 13 * 25) {
      return false;
    }

    it++;
  }

  //it = to_be_checked.begin();
  //while (it != to_be_checked.end()) {
  //  cout << *it << " ";
  //  it++;
  //}
  //cout << endl;

  return true;
}

char transform(char c, int r, char m[])
{
  return (m[(c-'a'+r) % 26] - 'a' - r + 26) % 26 + 'a';
}

char transform_1(char c, int r, char m[])
{
  for (int i = 0; i < 26; ++i) {
    if ((m[i] - r - 'a' + 26) % 26 + 'a' == c) {
      return 'a' + (i-r+26) % 26;
    }
  }

  return '?';
}

bool try_match(char pi[][27], char pir[], char perm[], char rotate[], char cipher[], char plaintext[])
{
  int l = strlen(cipher);
  int r0 = rotate[0] - 'a';
  int r1 = rotate[1] - 'a';
  int r2 = rotate[2] - 'a';
  int rr = rotate[3] - 'a';
  for (int i = 0; i < l; ++i) {
    char c = cipher[i];

    c = transform(c, 0, perm);
    c = transform(c, (r0+i%26) % 26, pi[0]);
    c = transform(c, (r1+i/26) % 26, pi[1]);
    c = transform(c, r2, pi[2]);
    c = transform(c, rr, pir);
    c = transform_1(c, r2, pi[2]);
    c = transform_1(c, (r1+i/26) % 26, pi[1]);
    c = transform_1(c, (r0+i%26) % 26, pi[0]);
    c = transform_1(c, 0, perm);

    if (c != plaintext[i]) {
      return false;
    }
  }

  return true;
}

string solve_case()
{
  char pi[3][27];
  char pir[27];
  char perm[27];
  char rotates[5];

  char plain[128];
  char cipher[128];

  cin >> pi[0] >> pi[1] >> pi[2] >> pir >> perm >> rotates;
  cin >> plain >> cipher;

  char *unknowns[3] = {NULL};
  set<int> unknown_pis;
  bool unknown_pir = false;
  bool unknown_perm = false;
  bool unknown_rotates = false;

  process_unknowns(unknowns,
      pi, pir, perm, rotates, plain,
      unknown_pis, unknown_pir, unknown_perm, unknown_rotates);

  int cnt_unknowns = 0;
  int cases = 1;
  for (cnt_unknowns = 0; cnt_unknowns < 3; cnt_unknowns++) {
    if (unknowns[cnt_unknowns] == NULL) {
      break;
    }

    cases *= 26;
  }

  int i, j;
  for (i = 0; i < cases; ++i) {
    int code = i;
    for (j = 0; j < cnt_unknowns; ++j) {
      *unknowns[j] = 'a' + code % 26;
      code /= 26;
    }

    if (!validate_settings(pi, pir, perm, unknown_pis, unknown_pir, unknown_perm)) {
      continue;
    }

    if (try_match(pi, pir, perm, rotates, cipher, plain)) {
      return plain;
    }
  }

  return "";
}

int main()
{
  int n;

  //char s[] = "bcdefghijklmnopqrstuvwxyza";
  //cout << transform('a', 0, s) << endl;
  //cout << transform('z', 0, s) << endl;
  //cout << transform('a', 1, s) << endl;
  //cout << transform('z', 1, s) << endl;
  //cout << transform_1('a', 1, s) << endl;
  //cout << transform_1('z', 1, s) << endl;

  cin >> n;
  for (int i = 1; i <= n; ++i) {
    printf("Scenario #%d:\n", i);
    cout << solve_case() << endl;
    cout << endl;
  }
}
