# part 1
import sys
import csv
import numpy as np
from cvxopt import matrix, solvers

# python port.py datafilename K N
if len(sys.argv) != 4:
    sys.exit("usage: port.py datafilename firstKassets firstNdays \n")
K = int(sys.argv[2])
N = int(sys.argv[3])

#open and read data file
try:
    csvfile = open(sys.argv[1],'rb')
    portreader = csv.reader(csvfile,skipinitialspace = True) # read the csv file and return a reader object
except IOError:
    sys.exit("Cannot open file %s\n" % sys.argv[1])

#read the datafile into numpy array
alist = []
day_length = 0
count = 0
for row in portreader:
    del row[-1]
    day_length = len(row)
    alist.append(row)
    count = count + 1
port_arr = np.array(alist).reshape(count, day_length)

#extract K assets for N days to form a new numpy array
target_port = port_arr[0:K, 0:N].astype(float)

#compute return and get an array of return
return_arr = np.empty((0,N-1))
for k in range(K):
    diff_asset = np.diff(target_port[k,:])
    temp = np.delete(target_port[k,:],-1)
    asset_return = np.divide(diff_asset,temp)
    return_arr = np.append(return_arr, [asset_return], axis=0)
average_return = np.mean(return_arr,axis=1)   #array of average return for K asset in N days
average_return = 100 * average_return   #set to 100%
return_arr = 100 * return_arr

#compute covariance of asset return
cov_port = np.cov(return_arr)

#output the covariance to file
average_return = np.around(average_return, decimals =2)
cov_port = np.around(cov_port, decimals=2)
f = open("large.txt", 'w')
f.write("n ")
f.write(str(K))
f.write("\n\nj_lower_upper_mu\n\n")
up = max(0.01, 2.0/K)

for k in range(K):
    f.write(str(k) + ' ' + '0 ' + str(up) + ' ' +str(average_return[k]) + '\n')
f.write('\n\nlambda 10\n\n')
f.write('covariance\n\n')
for i in range(K):
    for j in range(K):
        f.write(str(cov_port[i,j]) + '\t')
    f.write('\n')
f.write('\n\nEND')
f.close()
csvfile.close()


P = matrix(10*cov_port)
#print np.identity(K)
q = matrix(-1*average_return)
#print q
G = matrix(np.concatenate((-np.identity(K), np.identity(K)),0))
h = matrix(np.concatenate((np.zeros((K,1)), up*np.ones((K,1))),0))
A = matrix(1.0, (1,K))
b = matrix(1.0)

opt = open("opt_sol.csv", 'w')
sol = solvers.qp(P, q, G, h, A, b)
for x in sol["x"]:
    #print float(x)
    opt.write(str(x))
    opt.write('\n')


