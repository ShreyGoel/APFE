# -*- coding: utf-8 -*-
"""
Created on Sun Sep 23 16:05:42 2018

@author: Shrey
"""
from q2 import GetCov, GetPrices
from q1 import GetEigens
import numpy as np
import copy, time

prices = GetPrices()

matrix = GetCov(prices)

# input tolerance
tol = 0.01

refMatrix = copy.deepcopy(matrix)

#time the squared method
start = time.time()

matrix_squared = np.linalg.matrix_power(matrix, 2)
eigenValues, eigenVectors = GetEigens(matrix_squared, tol)
eigenValues = np.sqrt(eigenValues)

timer1 = time.time()
print('squared', timer1 - start)

#time the normal method
eigenValues1, eigenVectors1 = GetEigens(matrix, tol)
print('normal', time.time() - timer1)

F, V = np.linalg.eigh(refMatrix)