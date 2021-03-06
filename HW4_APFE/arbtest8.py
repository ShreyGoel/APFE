#!/usr/bin/python

import sys
from arbwriter import writelp
from mysolver import lpsolver

#if len(sys.argv) != 3:
#      sys.exit("usage: arbtest.py datafilename lpfilename\n")

#now open and read data file
try:
    datafile = open(sys.argv[1], 'r') # opens the data file "arb.dat"
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


sigmafile = sys.argv[2] # "example.dat"
sigma_file = open(sys.argv[2], 'r')
slines = sigma_file.readlines();
k = 0
sigmalist = []
while k < (numscen)*(numsec+1):
    linenow = slines[k].split()
    print(linenow)
    sigmalist.append(float(linenow[2]))
    k = k + 1




#now write LP file, now done in a separate function (should read data this way, as well)

lpwritecode = writelp("model.lp", p, numsec, numscen, sigmalist)

print("wrote LP to file", "model.lp","with code", lpwritecode)

#now solve lp 

lpsolvecode = lpsolver("model.lp", "model.log")

print("solved LP at", "model.lp","with code", lpsolvecode)
