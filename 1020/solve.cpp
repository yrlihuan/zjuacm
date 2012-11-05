#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <vector>
#include <iostream>
#include <list>

using namespace std;

#define MAX_LEN 21
#define MAX_LINE 81

inline bool is_letter(char a)
{
  return a>='A' && a<='Z';
}

char *dic;
vector<char*> lwords[MAX_LEN];

int find_next_match(char *searching, int prefix, vector<char*> words, int search_start, char *mapping_r)
{
  int search_end = words.size();
  if (prefix != 0) {
    char prefix_str[MAX_LEN];
    strcpy(prefix_str, searching);
    prefix_str[prefix] = '\0';

    int s = search_start;
    int e = words.size();

    while (s < e) {
      int m = (s + e) / 2;
      char *m_str = words[m];

      int cmp = strcmp(prefix_str, m_str);
      if (cmp < 0) {
        e = m;
      }
      else if (cmp > 0) {
        s = m + 1;
      }
      else {
        e = m;
        break;
      }
    }

    search_start = e;

    prefix_str[prefix] = 'Z' + 1;
    prefix_str[prefix+1] = '\0';

    s = search_start;
    e = words.size();

    if (s == e || strcmp(prefix_str, words[s]) < 0) {
      return -1;
    }

    while (s < e) {
      int m = (s + e) / 2;
      char *m_str = words[m];

      int cmp = strcmp(prefix_str, m_str);
      if (cmp < 0) {
        e = m;
      }
      else if (cmp > 0) {
        s = m + 1;
      }
      else {
        e = m + 1;
        break;
      }
    }

    search_end = e;
  }

  for (int i = search_start; i < search_end; ++i) {
    char *word = words[i] + prefix;
    char *p = searching + prefix;

    bool valid = true;
    while (*p) {
      if (*p == *word || (*p == '*' && mapping_r[*word] == '*')) {
        ++p;
        ++word;
      }
      else {
        valid = false;
        break;
      }
    }

    if (valid) {
      return i;
    }
  }

  return -1;
}

void solve_case()
{
  char *buf;
  bool first_line = true;

  vector<char*> encrypted;
  while (true) {
    buf = new char[MAX_LINE];
    gets(buf);
    if (!buf[0]) {
      if (!first_line)
        break;
    }
    else {
      first_line = false;

      char *p = buf;
      char *q = p;
      while (true) {
        bool reach_end = *p == '\0';
        if (*p == ' ' || *p == '\0') {
          *p = '\0';
          if (p - q > 0) {
            encrypted.push_back(q);
          }

          q = p + 1;

          if (reach_end)
            break;
        }

        ++p;
      }
    }
  }

  vector<char*> queue;
  vector<int> prefixes;
  list<char*> unqueued(encrypted.begin(), encrypted.end());
  char mapping[128];
  memset(mapping, 0, 128);

  while (unqueued.size()) {
    list<char*>::iterator it;
    list<char*>::iterator next;
    int next_score = 0;
    for (it = unqueued.begin(); it != unqueued.end(); ) {
      bool duplicate = false;
      for (unsigned int i = 0; i < queue.size(); ++i) {
        if (0 == strcmp(queue[i], *it)) {
          duplicate = true;
          break;
        }
      }

      if (duplicate) {
        it = unqueued.erase(it);
        continue;
      }

      int score = 0;
      int len = 0;
      char *p = *it;
      bool is_prefix = true;
      while (*p) {
        if (is_prefix && mapping[(int)*p]) {
          score += 10000;
        }
        else if (mapping[(int)*p]) {
          is_prefix = false;
          score += 5000;
        }

        ++len;
        ++p;
      }

      score += len;

      if (score > next_score) {
        next = it;
        next_score = score;
      }

      ++it;
    }

    if (next_score == 0) {
      continue;
    }

    int prefix = 0;
    char *p = *next;
    while (*p) {
      if (mapping[(int)*p]) {
        prefix++;
      }
      else {
        break;
      }

      ++p;
    }

    p = *next;
    while (*p) {
      mapping[(int)*p] = 'A';

      ++p;
    }

    queue.push_back(*next);
    prefixes.push_back(prefix);
    unqueued.erase(next);
  }

  // vector<char*>::iterator it;
  // for (it = queue.begin(); it < queue.end(); ++it) {
  //   cout << *it << endl;
  // }
  // cout << endl;

  memset(mapping, '*', 128);
  mapping['Z'+1] = '\0';

  char mapping_r[128];
  memset(mapping_r, '*', 128);
  mapping_r['Z'+1] = '\0';

  char result[27];

  int n_words = queue.size();

  char mapped_seq[26];
  int mapped_pos[n_words];

  int search_history[n_words];
  char *search_words[n_words];
  for (int i = 0; i < n_words; ++i) {
    search_words[i] = new char[MAX_LEN];
  }

  int pos = 0;
  int num_success = 0;
  int search_start = 0;
  int mapped_letters = 0;
  char *searching = NULL;

  while (true) {
    // If we reach a solution
    if (pos == n_words) {
      strcpy(result, mapping_r+'A');

      num_success++;
      if (num_success > 1) {
        break;
      }
      else {
        pos--;
        int mapped_cnt = mapped_pos[pos];
        for (int i = 0; i < mapped_cnt; ++i) {
          char en = mapped_seq[--mapped_letters];
          char de = mapping[(int)en];
          mapping[(int)en] = '*';
          mapping_r[(int)de] = '*';
        }

        search_start = search_history[pos] + 1;
        searching = search_words[pos];

        continue;
      }
    }

    char *word = queue[pos];
    int prefix = prefixes[pos];
    int len = 0;

    if (searching == NULL) {
      searching = search_words[pos];

      char *p = word;
      while (*p) {
        searching[len] = mapping[(int)*p];
        ++p;
        ++len;
      }

      searching[len] = '\0';
    }
    else {
      len = strlen(searching);
    }

    int ind = find_next_match(searching, prefix, lwords[len], search_start, mapping_r);
    // cout << searching << endl;
    // cout << prefix << endl;
    // cout << search_start << endl;
    // cout << ind << endl;
    // cout << mapping_r + 'A' << endl;
    if (ind >= 0) {
      char *decoded = lwords[len][ind];
      char mapping[26] = {0};
      char mapping_r[26] = {0};

      char *en = word;
      char *de = decoded;

      bool bad_attempt = false;
      while (*en) {
        if (!mapping[*en-'A'] && !mapping_r[*de-'A']) {
          mapping[*en-'A'] = *de;
          mapping_r[*de-'A'] = *en;
        }
        else if (mapping[*en-'A'] != *de || mapping_r[*de-'A'] != *en) {
          bad_attempt = true;
          break;
        }

        ++en;
        ++de;
      }

      if (bad_attempt) {
        search_start = ind + 1;
        continue;
      }
    }

    if (ind >= 0) {
      char *decoded = lwords[len][ind];
      // cout << decoded << endl;
      int mapped_cnt = 0;
      for (int i = prefix; i < len; ++i) {
        char de = decoded[i];
        char en = word[i];

        if (mapping[(int)en] == '*') {
          mapping[(int)en] = de;
          mapping_r[(int)de] = en;
          mapped_seq[mapped_letters++] = en;
          mapped_cnt++;
        }
      }

      mapped_pos[pos] = mapped_cnt;
      search_history[pos] = ind;

      search_start = 0;
      searching = NULL;
      pos++;
    }
    else {
      pos--;
      if (pos < 0) {
        break;
      }

      int mapped_cnt = mapped_pos[pos];
      for (int i = 0; i < mapped_cnt; ++i) {
        char en = mapped_seq[--mapped_letters];
        char de = mapping[(int)en];
        mapping[(int)en] = '*';
        mapping_r[(int)de] = '*';
      }

      search_start = search_history[pos] + 1;
      searching = search_words[pos];
    }
  }

  if (num_success == 0) {
    cout << "#No solution#" << endl;
  }
  else if (num_success == 1) {
    cout << result << endl;
  }
  else {
    cout << "#More than one solution#" << endl;
  }

  // cout << endl;
}

int main()
{
  int n;
  int i;
  dic = new char[1024 * 1024];

  for (i = 0; i < MAX_LEN; ++i) {
    lwords[i].reserve(2560);
  }

  char *p = dic;
  char *q = NULL;
  scanf("%d", &n);
  for (i = 0; i < n; ++i) {
    q = p;
    scanf("%s", p);

    while (*(p++))
      ;

    int len = p - q - 1;

    lwords[len].push_back(q);
  }

  scanf("%d", &n);

  for (i = 0; i < n; ++i) {
    solve_case();
  }
}
