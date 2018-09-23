# -*- coding: utf-8 -*-
"""
Created on Sat Sep 22 18:59:09 2018

python 3.6

"""

import numpy as np
import copy

# read matrix
matrix = np.genfromtxt(r"C:\Users\Shrey\Documents\GitHub\APFE\hw1\russell_cov.csv", delimiter = ',')

# input tolerance
tol = 0.01

refMatrix = copy.deepcopy(matrix)

def GetEigens(matrix, tol):
    matrixShape = matrix.shape[0]
    eigenValues = []
    eigenVectors = []
    
    # repeat until eigenvalue to biggest eigenvalue ratio goes below tolerance
    for i in range(matrixShape):
        eigenVector = np.random.normal(size=(matrixShape))
        
        # used for loop so that loop terminates in 100 iterations for sure
        for k in range(100):
            prevEigenVector = copy.deepcopy(eigenVector)
            eigenVector = np.matmul(matrix, eigenVector)
            eigenVector = eigenVector/np.linalg.norm(eigenVector)
            eigenValue = np.matmul(np.matmul(eigenVector.T, matrix), eigenVector)
            
            # break if norm of change in eigenVectors is below threshold
            if np.linalg.norm(eigenVector - prevEigenVector) <= 0.001:
                break
        
        eigenValues.append(eigenValue)
        eigenVectors.append(eigenVector)
        
        if eigenValue/eigenValues[0] > tol:
            x = np.outer(eigenVector, eigenVector)
            matrix = matrix - eigenValue * x
            
        else:
            break
    
    return eigenValues, eigenVectors

F, V = np.linalg.eigh(refMatrix)