# -*- coding: utf-8 -*-
"""
Created on Sun Sep 23 15:51:39 2018

python 3.6

"""
from q2 import GetCov, GetPrices
import numpy as np
import matplotlib.pyplot as plt

#convert to numpy array
prices = GetPrices()

nPrices = prices.shape[0]

ranks = [np.nan] * (nPrices - 2)

for t in range(2, nPrices-1):
    cov = GetCov(prices, t)
    rank = np.linalg.matrix_rank(cov)
    ranks[t-2] = rank

plt.plot(list(range(2, nPrices)), ranks)