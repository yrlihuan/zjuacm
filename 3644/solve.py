#!/usr/bin/env python

import os
import sys

solutions = 0
step_cache = None
current_lcm = None
target = None
scores = None
target_n = None
valid_points = None

def gcd(a, b):
  """Return greatest common divisor using Euclid's Algorithm."""
  while b:      
      a, b = b, a % b
  return a

def lcm(a, b):
  """Return lowest common multiple."""
  return a * b // gcd(a, b)

def visit(p):
  global solutions, current_lcm

  value = lcm(current_lcm, scores[p])
  step_cache.append(value)
  current_lcm = value

  #print "visit %d" % p
  #print scores[p]
  #print value
  #print target_n

  if step_cache[-1] == step_cache[-2]:
    return False

  if p == target_n:
    if value == target:
      solutions += 1

    return False

  if not valid_points[p]:
    return False

  return value < target

def rollback(p):
  global current_lcm

  step_cache.pop()
  current_lcm = step_cache[-1]

  #print "rollback %d" % p
  #print current_lcm

def graph_walk(conn):
  n = len(conn)
  visited = [False] * n
  path = []
  search_index = [-1] * n

  p = 0

  while True:
    visited[p] = True
    path.append(p)
    valid = visit(p)
    next_p = -1

    if valid:
      candidates = conn[p]
      ind = 0
      while len(candidates) > ind:
        c = candidates[ind][0]
        if not visited[c]:
          next_p = c
          search_index[p] = ind
          break
        else:
          ind += 1

    if valid and next_p >= 0:
      p = next_p
    else:
      rollback_done = False
      visited[p] = False
      path.pop()
      rollback(p)

      while len(path) > 0:
        last_p = path[-1]
        candidates = conn[last_p]
        ind = search_index[last_p] + 1

        while len(candidates) > ind:
          c = candidates[ind][0]
          if not visited[c]:
            search_index[last_p] = ind
            p = c
            rollback_done = True
            break

        if rollback_done:
          break
        else:
          search_index[last_p] = -1
          visited[last_p] = False
          path.pop()
          rollback(last_p)

      if not rollback_done:
        #print path
        #print visited
        #print search_index
        break

def build_data(lines, start):
  p = start
  n, m, k = lines[p].split()
  n = int(n)
  m = int(m)
  target = int(k)

  n_lines = m + 2
  conn = []
  for i in xrange(0,n):
    conn.append([])

  p += 1
  for i in xrange(0, m):
    f, t = lines[p].split()
    f = int(f) - 1
    t = int(t) - 1

    conn[f].append((t, 1))
    p += 1

  scores = []
  for s in lines[p].split():
    scores.append(int(s))

  return conn, scores, target, n_lines

def solve():
  lines = sys.stdin.readlines()
  p_lines = 0

  global scores, target, solutions, step_cache, target_n, current_lcm, valid_points

  while p_lines < len(lines):
    conn, scores, target, n_lines = build_data(lines, p_lines)
    solutions = 0
    step_cache = [0]
    target_n = len(conn) - 1
    current_lcm = 1

    valid_points = []
    for s in scores:
      valid_points.append(gcd(s, target) == s)

    graph_walk(conn)
    print solutions

    p_lines += n_lines

if __name__ == "__main__":
  solve()
