# -*- coding: utf-8 -*-
"""
Created on Sun Oct 14 23:56:34 2018

"""

import numpy as np
import pandas as pd

def read_file(fp):
    fp = "example.txt"
    text_file = open(fp, "r")
    lines = text_file.readlines() 
    lines = [s.replace("\n", "") for s in lines]
    #print(lines)

    col_names = lines[2].split("_")
    st = 4
    mu_mat = pd.DataFrame(columns=col_names)

    for i in range(4):
        tmp = []
        mu_vec = lines[st + i]
        line_vec = mu_vec.split(" ")
        for j in line_vec:
            if j != '':
                tmp.append(float(j))
                #    print(tmp)
        mu_mat.loc[i] = pd.Series({'j': tmp[0], 'lower': tmp[1], 'upper':
                                    tmp[2], 'mu': tmp[3]})

    #print(mu_mat)
    
    stq = 13
    q_mat =  pd.DataFrame(columns=['0', '1', '2', '3'])
         
    for i in range(4):
        tmp = []
        q_vec = lines[stq + i]
        line_vec = q_vec.split(" ")
        for j in line_vec:
            if j != '':
                tmp.append(float(j))
            #    print(tmp)
        q_mat.loc[i] = pd.Series({'0': tmp[0], '1': tmp[1], '2':
                                  tmp[2], '3': tmp[3]})
 
    
    
    lamda = lines[9]
    #print(q_mat)
    dfs = {}
    dfs[0] = lamda
    dfs[1] = mu_mat
    dfs[2] = q_mat
    return dfs



def feasibility(lower, upper):
    sum_l = 0
    for s in lower:
        sum_l = sum_l + s
    sum_u = 0
    for s in upper:
        sum_u = sum_u + s

    retval = 0
    if sum_l < 1 and sum_u > 1:
        retval = 1
    
    return(retval)


def initialize(lower, upper):
    x = lower
    bracket = upper - lower
    bracket = bracket.tolist()
    i = 0
    while sum(x) < 1:
        x_i = x[i]
        inc = round(1 - sum(x), 5)
        if bracket[i] > inc:
            x[i] = x_i + inc
        else:
            x[i] = x_i + bracket[i]
        i = i + 1

    return x


def gradient(mu, q, lamda):
    return [2*lamda*num1-num2 for num1,num2 in zip(np.matmul(q, x),mu)]


def algo(gradient, mu):
    gr = np.array(gradient)
    sort_index = np.argsort(gr)[::-1]
    sort_array = np.sort(gr)[::-1]
    x_sort = [x[i] for i in sort_index]
    l_sort = [lower[i] for i in sort_index]
    u_sort = [upper[i] for i in sort_index]

    m = range(0, len(x))
    res_check = []

    for i in m:
        l_l = [j for j in m if j < i]
        u_l = [j for j in m if j > i]
        sum_all = 0

        if len(l_l) > 0:
            l_b = [l_sort[j] for j in l_l]
            x_b = [x_sort[j] for j in l_l]
            l_a = [num1-num2 for num1,num2 in zip(l_b,x_b)]
            sum_all = sum_all + np.sum(l_a)
        else:
            l_a = []
        
        if len(u_l) > 0:
            u_b = [u_sort[j] for j in u_l]
            x_b = [x_sort[j] for j in u_l]
            u_a = [num1-num2 for num1,num2 in zip(u_b,x_b)]
            sum_all = sum_all + np.sum(u_a)
        else:
            u_a = []        
        
        y_rem = (-1)*sum_all

        if y_rem >= (l_sort[i] - x_sort[i]) and y_rem <= (u_sort[i] - x_sort[i]): 
            y_res = l_a + [y_rem] + u_a
            res_check.append(y_res)     



# if(len(res_check) != 1):

    if(len(res_check) > 0):
        comp_descent = []
        for arr in res_check:
            resort_index = np.argsort(sort_index)
            array_ret = [arr[i] for i in resort_index]
            comp_descent.append(np.matmul(sort_array, array_ret))
        val, idx = min((val, idx) for (idx, val) in enumerate(comp_descent))

        retval = [res_check[idx][i] for i in resort_index]
    else:
        print("Problem not feasible")
        retval = 0
    return(retval)

def step_length(y, mu, q, x, lamda):
    num = np.matmul(mu, y) - 2*lamda*np.matmul(y, np.matmul(q, x))
    den = 2*lamda*np.matmul(y, np.matmul(q, y))
    step = num/den
    if step > 1:
        step = 1
    if step < 0:
        step = 0
    return step

if __name__ == '__main__':

    retval = read_file("example.txt")

    lamda = int(retval[0].split(" ")[1])
    mu = retval[1]
    q = retval[2]
    n_eigs = 2
    
    eig_vals, eig_vecs = np.linalg.eig(q)
    eig_vecs = eig_vecs[:n_eigs,:]
    eig_vals = eig_vals[:n_eigs]
    
    D = np.diag(np.sum(eig_vecs.T, axis = 0))
    
    F = np.diag(eig_vals)
    
    D_inv = np.linalg.inv(D)
    q = np.matmul(np.matmul(D_inv, F), D_inv)
    lower = np.matmul(np.matmul(D, eig_vecs), mu['lower'].values.T)
    upper = np.matmul(np.matmul(D, eig_vecs), mu['upper'].values.T)

    mu = np.matmul(np.matmul(mu['mu'].values.T, eig_vecs.T), D_inv)    
    
    if feasibility(lower, upper):
        x = initialize(lower, upper)
        counter = 0
        step = 1

        while step > 0.000001 and counter < 100:
            counter = counter + 1
            g = gradient(mu,q,lamda)
            y = algo(g, mu)
            if y == 0:
                break
            step = step_length(y, mu, q, x, lamda)
            delta = [step*i for i in y]
            tmp = [num1+num2 for num1,num2 in zip(x,delta)]
            x = tmp
    
    print(x)
    print(sum(x))
    print(counter)