#!/usr/bin/env python
import sys
import random
import string

def gen_coder():
  coder = 'A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z'.split(',')

  num = 10000
  for i in xrange(0, num):
    x = random.randint(0,25)
    y = random.randint(0,25)

    if x != y:
      cx = coder[x]
      cy = coder[y]

      coder[x] = cy
      coder[y] = cx

  return ''.join(coder)

def gen_voc(voc_size):
  char = 'A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z'.split(',')

  voc = {}
  values = set()
  p = 0
  for i in xrange(0, voc_size):
    l = random.randint(2, 20)

    w = []
    for j in xrange(0, l):
      w.append(random.choice(char))

    w = ''.join(w)

    if w not in values:
      voc[p] = w
      values.add(w)
      p += 1

  return voc

def gen_case(voc, coder):
  words = []
  trans = string.maketrans('ABCDEFGHIJKLMNOPQRSTUVWXYZ', coder)

  l = random.randint(3, 40)
  for i in xrange(0, l):
    original = random.choice(voc)
    coded = original.translate(trans)

    words.append(coded)

  return words

def print_data(voc, cases):
  print len(voc)

  voc = voc.values()
  voc.sort()

  for v in voc:
    print v

  print len(cases)
  for c in cases:
    print ''
    for w in c:
      print w

if __name__ == "__main__":
  if len(sys.argv) < 3:
    print "casegen.py <vocabulary size> <number cases>"
    sys.exit(1)

  voc_size = int(sys.argv[1])
  num_cases = int(sys.argv[2])

  voc = gen_voc(voc_size)
  cases = []
  coders = []
  for i in xrange(0, num_cases):
    coder = gen_coder()
    coders.append(coder)
    cases.append(gen_case(voc, coder))

  print_data(voc, cases)

  print ''
  for c in coders:
    print c


