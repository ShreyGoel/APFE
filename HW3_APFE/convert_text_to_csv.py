#!/usr/bin/python

import numpy as np
from numpy import linalg as LA
import sys
import math
# import random
import time
import csv

# if len(sys.argv) != 2:  # the program name and the datafile
#     # stop the program and print an error message
#     sys.exit("usage: eigen.py datafile ")

# filename = sys.argv[1]

# print "input", sys.argv[1]

filename = "russell_prices.txt"
try:
    f = open(filename, 'r')
except IOError:
    print ("Cannot open file %s\n" % filename)
    sys.exit("bye")

# read data
data = f.readlines()
f.close()

nrows = len(data) - 2
print "read", nrows, "rows"

# create the covariance matrix from file


row = data[0]
line = row.split()
num_eig = int(line[3])
print "num_eig =", num_eig

sigma = np.zeros ((nrows, len(data[3].split())))  


for i in xrange(nrows):
    row = data[2 + i]
    line = row.split()
    ndays = len(line)
    print row
    for j in xrange(ndays):
        sigma[i][j] = float(line[j])
 

print sigma
with open("russell_prices.csv","w+") as my_csv:
    csvWriter = csv.writer(my_csv,delimiter=',')
    csvWriter.writerows(sigma)