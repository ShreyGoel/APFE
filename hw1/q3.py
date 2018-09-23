# -*- coding: utf-8 -*-
"""
Created on Sun Sep 23 15:51:39 2018

python 3.6

"""

from PowerMethod import GetEigens
from q2 import GetCov, GetPrices

#convert to numpy array
prices = GetPrices()

nPrices = prices.shape[0]

covs = []
eigenValues = []
eigenVectors = []

for t in range(2, nPrices):
    cov = GetCov(prices, t)
    eigenValue, eigenVector = GetEigens(cov, 0.01)
    
    covs.append(cov)
    eigenValues.append(eigenValue)
    eigenVectors.append(eigenVector)
    
