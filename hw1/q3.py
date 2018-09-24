# -*- coding: utf-8 -*-
"""
Created on Sun Sep 23 15:51:39 2018

python 3.6

"""

from PowerMethod import GetEigens
from q2 import GetCov, GetPrices
import numpy as np
import itertools
import matplotlib.pyplot as plt

#convert to numpy array
prices = GetPrices()

nPrices = prices.shape[0]

eigenValues = [np.nan] * (nPrices - 2)

for t in range(2, nPrices):
    cov = GetCov(prices, t)
    
    eigenValue, eigenVector = GetEigens(cov, 1)
#    eigenValue = np.sqrt(eigenValue)
    print(t)
    eigenValues[t-2] = eigenValue

eigenValues = list(itertools.chain(*eigenValues))

plt.plot(list(range(nPrices - 2)), eigenValues)