# -*- coding: utf-8 -*-
"""
Created on Sat Sep 22 22:11:31 2018

python 3.6

@author: Shrey
"""
import numpy as np
import pandas as pd
import PowerMethod

with open('missing.dat') as f:
    data = [line.split() for line in f]  
    
metrics = data[0]
prices = pd.DataFrame(data[1:]).T
prices = prices.replace('NA', np.nan).fillna(method = 'ffill').iloc[:,:-1].astype(float)

prices = prices.fillna(prices.mean())
prices = prices.values

def GetCov(data, T):
    returns = (data[1:] - data[:-1])/data[1:]
    
    returns = returns[:-T]
    
    meanReturns = np.mean(returns, axis = 0)
    
    returns = returns - meanReturns[np.newaxis, :]
    nReturns = returns.shape[1]
    cov = np.zeros((nReturns, nReturns))
    
    for i in range(nReturns):
        for j in range(nReturns):
            cov[i,j] = np.mean(returns[:,i]*returns[:,j])
    
    return cov

cov = GetCov(prices, 10)
