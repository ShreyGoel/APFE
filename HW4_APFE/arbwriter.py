#!/usr/bin/python

import sys
def writelp(lpfilename, prices, numsec, numscen, sigma):

    try:
        lpfile = open(lpfilename, 'w') # opens the file
    except IOError:
        print("Cannot open LP file %s for writing\n" % lpfilename)
        return 1

    print("now writing LP to file")
    lpfile.write("Minimize ")
    j = 0
    counter = 0

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

        lpfile.write("scen_" + str(counter) +": ")
        j = 0
        counter = counter + 1
        while j <= numsec:
            index = k*(1 + numsec) + j
            index_sigma = j
            if prices[index]-sigma[j] >= 0:
                lpfile.write("+ ")
                lpfile.write(str(abs(prices[index]-sigma[j])) + " u" + str(k)+ str(j)+" ")
            else:
                lpfile.write("- ")
                lpfile.write(str(abs(prices[index]-sigma[j])) + " u" + str(k)+ str(j)+" ")
            if -prices[index]-sigma[j] >= 0:
                lpfile.write("+ ")
                lpfile.write(str(abs(-prices[index]-sigma[j])) + " v" + str(k)+ str(j)+" ")
            else:
                lpfile.write("- ")
                lpfile.write(str(abs(-prices[index]-sigma[j])) + " v" + str(k)+ str(j)+" ")
            


            j += 1
        lpfile.write(" >= 0\n")
        k += 1

    k = 0
    
    while k <= numscen:
        # write constraint for scenario k
        
        j = 0
        while j <= numsec:
            #index = k*(1 + numsec) + j
            index_sigma = j
            lpfile.write("scen_" + str(counter) +": ")    
            
            lpfile.write("+ ")
            lpfile.write( " u" + str(k)+ str(j)+" - "+" v" + str(k)+ str(j)+" = x"+ str(j)+"\n")
            
            counter = counter + 1 

            j += 1
        #lpfile.write(" >= 0\n")
        k += 1



    lpfile.write("Bounds\n")
    j = 0
    while j <= numsec:
        lpfile.write("-1 <= x" + str(j) + " <= 1\n")
        j += 1
    

    k=0
    while k <= numscen:
        # write constraint for scenario k
        #lpfile.write("scen_u_v" + str(k+numsec) +": ")
        j = 0
        while j <= numsec:
            #index = k*(1 + numsec) + j
            index_sigma = j
            
            
            
            lpfile.write( " u" + str(k)+ str(j)+" >= 0\n")
            lpfile.write( " v" + str(k)+ str(j)+" >= 0\n")


    


            j += 1
        #lpfile.write(" >= 0\n")
        k += 1


    lpfile.write("End\n")

    print("closing lp file")
    lpfile.close()
      
    return 0

