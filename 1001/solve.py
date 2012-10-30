import os
import sys

def solve():
  for line in sys.stdin.readlines():
    a,b = line.split()
    print int(a) + int(b)

if __name__ == "__main__":
  solve()
