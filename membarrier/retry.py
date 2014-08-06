#!/usr/bin/env python

import sys
import subprocess

# make
subprocess.check_call('make')

if len(sys.argv) == 1:
    loops = 1000
else:
    loops = int(sys.argv[1])

print 'looping for %s times' % loops

zeros = 0
for i in xrange(loops):
    s = subprocess.check_output('./test')
    if s == '0':
        zeros += 1

print 'get %s zeros' % zeros
