# -*- coding: utf-8 -*-
"""
Created on Sun Sep 23 16:05:42 2018

@author: Shrey
"""
from q2 import GetCov, GetPrices
import numpy as np
import copy

prices = GetPrices()

matrix = GetCov(prices)

# input tolerance
tol = 0.001

refMatrix = copy.deepcopy(matrix)

def GetEigens(matrix, tol):
    matrixShape = matrix.shape[0]
    eigenValues = []
    eigenVectors = []
    
    # repeat until eigenvalue to biggest eigenvalue ratio goes below tolerance
    for i in range(matrixShape):
        eigenVectorRandom = np.random.normal(size=(matrixShape))
        
        # used for loop so that loop terminates in 100 iterations for sure
        for k in range(100):
            
            if k==0:
                prevEigenVector = copy.deepcopy(eigenVectorRandom)
            else:
                prevEigenVector = copy.deepcopy(eigenVector)
                matrix = np.matmul(matrix, matrix)

            eigenVector = np.matmul(matrix, eigenVectorRandom)
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

eigenValues, eigenVectors = GetEigens(matrix, tol)
F, V = np.linalg.eigh(refMatrix)