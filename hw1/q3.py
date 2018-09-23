# -*- coding: utf-8 -*-
"""
Created on Sun Sep 23 15:51:39 2018

python 3.6

"""

from PowerMethod import GetEigens
from q2 import GetCov
import numpy as np
import pandas as pd

#read file
with open('missing.dat') as f:
    prices = [line.split() for line in f]  

# clean matrix 
metrics = prices[0]
prices = pd.DataFrame(prices[1:]).T

#use last available price to fill nans
prices = prices.replace('NA', np.nan).fillna(method = 'ffill').iloc[:,:-1].astype(float)

#use mean price for corner cases
prices = prices.fillna(prices.mean())

#convert to numpy array
prices = prices.values

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
    
