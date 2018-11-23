#!/usr/bin/python

import sys

def writelp(lpfilename, prices, numsec, numscen):

    try:
        lpfile = open(lpfilename, 'w') # opens the file
    except IOError:
        print("Cannot open LP file %s for writing\n" % lpfilename)
        return 1

    print("now writing LP to file")
    lpfile.write("Minimize ")
    j = 0
    while j <= numsec:
        if prices[j] >= 0:
            lpfile.write("+ ")
        lpfile.write(str(prices[j]) + " x" + str(j)+" ")
        # str converts argument into string;  " +" concatenates strings
        j += 1
    lpfile.write("\nSubject to\n")

    k = 0
    while k <= numscen:
        # write constraint for scenario k
        lpfile.write("scen_" + str(k) +": ")
        j = 0
        while j <= numsec:
            index = k*(1 + numsec) + j
            if prices[index] >= 0:
                lpfile.write("+ ")
                lpfile.write(str(prices[index]) + " x" + str(j)+" ")
            j += 1
        lpfile.write(" >= 0\n")
        k += 1

    lpfile.write("Bounds\n")
    j = 0
    while j <= numsec:
        lpfile.write("-1 <= x" + str(j) + " <= 1\n")
        j += 1
    lpfile.write("End\n")

    print("closing lp file")
    lpfile.close()
      
    return 0

