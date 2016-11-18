#!/usr/bin/env python3

#    MPIR Benchmark Program v0.6
#
#    Copyright 2016 Alexander Kruppa
#
#    This program is free software; you can redistribute it and/or modify
#    it under the terms of version 2.1 of the GNU General Public License 
#    as published by the Free Software Foundation; it is not distributable 
#    under version 3 (or any later version) of the GNU General Public License.
#    
#    This program is distributed in the hope that it will be useful, 
#    but WITHOUT ANY WARRANTY; without even the implied warranty of 
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
#    GNU General Public License for more details.

# Collates output from multiple bench_two runs into one
# The combined value is the minimum value among the input files

import sys

def split(line, delim):
  if delim[0] not in line:
    return [line]
  return [split(w, delim[1:]) for w in line.split(delim[0])]

def add(a, b, func):
  r = list()
  for n,m in zip(a,b):
    if type(n[0]) is list:
      r.append(add(n, m, func))
    else:
      u = float(n[0])
      if u.is_integer():
        u = int(u)
      v = float(m[0])
      if v.is_integer():
        v = int(v)
      r.append([func([u,v])])
  return(r)

def join(l, delim):
  r = None
  for i in l:
    if type(i) is list:
      i = join(i, delim[1:])
    if r is None:
      r = str(i)
    else:
      r = r + delim[0] + str(i)
  return r

alldata = list()
for filename in sys.argv[1:]:
  with open(filename) as f:
    alldata.append(f.readlines())

nrLines = len(alldata[0])
for filedata in alldata:
  assert len(filedata) == nrLines

for iLine in range(nrLines):
  total = []
  for filedata in alldata:
    line = filedata[iLine]
    s = split(line, ("=>", ",", ","))
    if len(total) == 0:
      total = s
    else:
      if len(s) == 1:
        if not total == s:
          print("Lines differ")
      else:
        assert total[0] == s[0]
        # Combine values from this file with the totals
        # using the lowest value (min)
        total = [total[0]] + add(total[1:], s[1:], min)
  print(join(total, (" => ", ",", ",")))
