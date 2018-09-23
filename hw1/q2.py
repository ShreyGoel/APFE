# -*- coding: utf-8 -*-
"""
Created on Sat Sep 22 22:11:31 2018

python 3.6

"""
import numpy as np
import pandas as pd
from PowerMethod import GetEigens

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

def GetCov(data, T = None):
    # get returns
    returns = (data[1:] - data[:-1])/data[1:]
    
    #return only T days of returns data
    if T:
        returns = returns[:-T]
    
    #remove mean of returns from returns
    meanReturns = np.mean(returns, axis = 0)
    returns = returns - meanReturns[np.newaxis, :]
    
    #initialize numpy array for loop
    nReturns = returns.shape[1]
    cov = np.zeros((nReturns, nReturns))
    
    # fill the covariance matrix
    for i in range(nReturns):
        for j in range(i, nReturns):
            cov[i,j] = np.mean(returns[:,i]*returns[:,j])
    
    out = cov.T + cov
    np.fill_diagonal(out,np.diag(cov))
    
    return out

cov = GetCov(prices)

eigenValue, eigenVector = GetEigens(cov, 0.01)