#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <vector>
#include <iostream>
#include <list>

using namespace std;

typedef struct TypeJob
{
  int m;
  int n;

  int arrive;
  int deadline;

  int reward;
  int bonus;
  int punishment;
} Job;

int job_compare(const void *a, const void *b)
{
  Job *ja = (Job*)a;
  Job *jb = (Job*)b;

  if (ja->arrive == jb->arrive)
    return jb->reward - ja->reward;
  else
    return ja->arrive - jb->arrive;
}

int run(Job job, int t)
{
  int income = job.reward;
  int deadline = job.deadline;

  if (deadline > t + 1) {
    income += job.bonus * (deadline-t-1);
  }
  else {
    income += job.punishment * (deadline-t-1);
  }

  // printf("%d\n", income);
  return income;
}

int case_solve(int f)
{
  int m;
  int n;
  int l;
  cin >> m >> n >> l;

  Job *jobs = new Job[l];

  int i;
  for (i = 0; i < l; ++i) {
    Job *p = jobs + i;

    scanf("%d %d %d %d %d %d %d", &p->m, &p->n, &p->arrive, &p->deadline, &p->reward, &p->bonus, &p->punishment);
  }

  qsort(jobs, l, sizeof(Job), job_compare);
  list<Job> queue;

  int income = 0;
  int p = 0;

  int t = 0;
  for (t = 0; t < f; ++t) {
    int tm = m;
    int tn = n;

    list<Job>::iterator it;
    for (it = queue.begin(); it != queue.end(); ++it) {
      if (it->m <= tm && it->n <= tn) {
        tm -= it->m;
        tn -= it->n;

        income += run(*it, t);
        it = queue.erase(it);
        --it;
      }
    }

    while (jobs[p].arrive == t) {
      if (jobs[p].m <= tm && jobs[p].n <= tn) {
        tm -= jobs[p].m;
        tn -= jobs[p].n;

        income += run(jobs[p], t);
      }
      else {
        queue.push_back(jobs[p]);
      }

      ++p;
    }
  }

  while (jobs[p].arrive == t) {
    queue.push_back(jobs[p]);
    ++p;
  }

  list<Job>::iterator it;
  for (it = queue.begin(); it != queue.end(); ++it) {
    if (it->deadline <= t) {
      int delay = t - it->deadline;
      income -= it->punishment * delay;
    }
  }

  delete []jobs;

  return income;
}

int main()
{
  int f;
  int cnt = 0;
  while (scanf("%d", &f) != EOF) {
    if(!f)
      break;

    ++cnt;

    printf("Case %d: %d\n", cnt, case_solve(f));
    printf("\n");
  }
}


