#!/usr/bin/python

import sys
from arbwriter import writelp
from mysolver import lpsolver

#if len(sys.argv) != 3:
#    sys.exit("usage: arbtest.py datafilename lpfilename\n")

#now open and read data file
try:
    datafile = open("arb.dat", 'r') # opens the data file
except IOError:
    sys.exit("Cannot open file")


lines = datafile.readlines();
datafile.close()

#print lines[0]
firstline = lines[0].split()
#print "first line is", firstline

numsec = int(firstline[1])
numscen = int(firstline[3])
r = float(firstline[5])
print("\n")
print("number of securities:", numsec,"number of scenarios", numscen,"r",r)
print ("\n")

#allocate prices as one-dim array
total = (1 + numsec)*(1 + numscen)
print ("total allocation:", total)
p = [0]*total
k = 0
# line k+1 has scenario k (0 = today)
while k <= numscen:
    thisline = lines[k + 1].split()
    # should check that the line contains numsec + 1 words

    p[k*(1 + numsec)] = 1 + r*(k != 0) # handles the price of cash
    j = 1
    while j <= numsec:
        value = float(thisline[j])
        p[k*(1 + numsec) + j] = value
        # print ">>", "k",k, "j",j, k*(1 + numsec) + j
        j += 1
    k += 1

#now write LP file, now done in a separate function (should read data this way, as well)

lpwritecode = writelp("demo.lp", p, numsec, numscen)

print("wrote LP to file", "demo.lp","with code", lpwritecode)

#now solve lp 

lpsolvecode = lpsolver("demo.lp", "test.log")

print("solved LP at", "demo.lp","with code", lpsolvecode)
