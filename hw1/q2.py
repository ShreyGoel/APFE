# -*- coding: utf-8 -*-
"""
Created on Sat Sep 22 22:11:31 2018

python 3.6

"""
import numpy as np
import pandas as pd
from PowerMethod import GetEigens

def GetPrices():
    #read file
    with open('missing.dat') as f:
        prices = [line.split() for line in f]  
    
    # clean matrix 
    prices = pd.DataFrame(prices[1:]).T
    
    #use last available price to fill nans
    prices = prices.replace('NA', np.nan).fillna(method = 'ffill').iloc[:,:-1].astype(float)
    
    #use mean price for corner cases
    prices = prices.fillna(prices.mean())
    
    #convert to numpy array
    prices = prices.values
    return prices

def GetCov(data, T = None):
    # get returns
    returns = (data[1:] - data[:-1])/data[1:]
    
    #return only T days of returns data
    if T:
        returns = returns[:-T+1]
    
    #remove mean of returns from returns
    meanReturns = np.mean(returns, axis = 0)
    returns = returns - meanReturns[np.newaxis, :]
    
    #initialize numpy array for loop
    nReturns = returns.shape[1]
    
    cov = (1/nReturns) * np.matmul(returns.T, returns)
    
    return cov

cov = GetCov(GetPrices())

eigenValue, eigenVector = GetEigens(cov, 0.01)