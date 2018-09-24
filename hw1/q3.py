# -*- coding: utf-8 -*-
"""
Created on Sun Sep 23 15:51:39 2018

python 3.6

"""

from PowerMethod import GetEigens
from q2 import GetCov, GetPrices
import numpy as np

#convert to numpy array
prices = GetPrices()

nPrices = prices.shape[0]

eigenValues = []
eigenVectors = []

for t in range(2, nPrices):
    cov = GetCov(prices, t)
    
    eigenValue, eigenVector = GetEigens(np.linalg.matrix_power(cov, 2), 0.01)
    eigenValue = np.sqrt(eigenValue)
    
    eigenValues.append(eigenValue)    
