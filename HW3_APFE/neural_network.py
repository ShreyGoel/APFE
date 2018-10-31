import pandas as pd
import os
import sys
import numpy as np
import h5py
import matplotlib.pyplot as plt
from dnn_utils import *

#matplotlib inline
plt.rcParams['figure.figsize'] = (5.0, 4.0) # set default size of plots
plt.rcParams['image.interpolation'] = 'nearest'
plt.rcParams['image.cmap'] = 'gray'


np.random.seed(1)

def read_data(fn):
	dat = pd.read_csv(fn, header = None)
	return dat


def initialize_parameters(n_x, n_h, n_y):
    
    np.random.seed(1)
    
    W1 = np.random.randn(n_h, n_x) * 0.01
    b1 = np.zeros(shape=(n_h, 1))
    W2 = np.random.randn(n_y, n_h) * 0.01
    b2 = np.zeros(shape=(n_y, 1))
    
    assert(W1.shape == (n_h, n_x))
    assert(b1.shape == (n_h, 1))
    assert(W2.shape == (n_y, n_h))
    assert(b2.shape == (n_y, 1))
    
    parameters = {"W1": W1,
                  "b1": b1,
                  "W2": W2,
                  "b2": b2}
    
    return parameters    


def initialize_parameters_deep(layer_dims):
    
    np.random.seed(3)
    parameters = {}
    L = len(layer_dims)            # number of layers in the network

    for l in range(1, L):
        ### START CODE HERE ### (≈ 2 lines of code)
        parameters['W' + str(l)] = np.random.randn(layer_dims[l], layer_dims[l - 1]) * 0.01
        parameters['b' + str(l)] = np.zeros((layer_dims[l], 1))
        ### END CODE HERE ###
        
        assert(parameters['W' + str(l)].shape == (layer_dims[l], layer_dims[l - 1]))
        assert(parameters['b' + str(l)].shape == (layer_dims[l], 1))

        
    return parameters

def linear_forward(A, W, b):
    """
    Implement the linear part of a layer's forward propagation.

    Arguments:
    A -- activations from previous layer (or input data): (size of previous layer, number of examples)
    W -- weights matrix: numpy array of shape (size of current layer, size of previous layer)
    b -- bias vector, numpy array of shape (size of the current layer, 1)

    Returns:
    Z -- the input of the activation function, also called pre-activation parameter 
    cache -- a python dictionary containing "A", "W" and "b" ; stored for computing the backward pass efficiently
    """
    # print(W.shape)
    # print(A.shape)
    # print(b.shape)
    
    Z = np.dot(W, A) + b
    
    assert(Z.shape == (W.shape[0], A.shape[1]))
    cache = (A, W, b)
    
    return Z, cache



def linear_activation_forward(A_prev, W, b, activation = "sigmoid"):
    """
    Implement the forward propagation for the LINEAR->ACTIVATION layer

    Arguments:
    A_prev -- activations from previous layer (or input data): (size of previous layer, number of examples)
    W -- weights matrix: numpy array of shape (size of current layer, size of previous layer)
    b -- bias vector, numpy array of shape (size of the current layer, 1)
    activation -- the activation to be used in this layer, stored as a text string: "sigmoid" or "relu"

    Returns:
    A -- the output of the activation function, also called the post-activation value 
    cache -- a python dictionary containing "linear_cache" and "activation_cache";
             stored for computing the backward pass efficiently
    """
    
    if activation == "sigmoid":
        # Inputs: "A_prev, W, b". Outputs: "A, activation_cache".
        Z, linear_cache = linear_forward(A_prev, W, b)
        A, activation_cache = sigmoid(Z)
    
    elif activation == "relu":
        Z, linear_cache = linear_forward(A_prev, W, b)
        A, activation_cache = relu(Z)
    
    assert (A.shape == (W.shape[0], A_prev.shape[1]))
    cache = (linear_cache, activation_cache)


    return A, cache



def L_model_forward(X, parameters):
    """
    Implement forward propagation for the [LINEAR->RELU]*(L-1)->LINEAR->SIGMOID computation
    
    Arguments:
    X -- data, numpy array of shape (input size, number of examples)
    parameters -- output of initialize_parameters_deep()
    
    Returns:
    AL -- last post-activation value
    caches -- list of caches containing:
                every cache of linear_relu_forward() (there are L-1 of them, indexed from 0 to L-2)
                the cache of linear_sigmoid_forward() (there is one, indexed L-1)
    """

    caches = []
    A = X
    L = len(parameters) // 2                  # number of layers in the neural network
        
    for l in range(1, L):
        A_prev = A
        ### START CODE HERE ### (≈ 2 lines of code)
        A, cache = linear_activation_forward(A_prev, 
                                             parameters['W' + str(l)], 
                                             parameters['b' + str(l)], 
                                             activation='sigmoid')
        caches.append(cache)

    
    AL, cache = linear_activation_forward(A, 
                                          parameters['W' + str(L)], 
                                          parameters['b' + str(L)], 
                                          activation='sigmoid')
    caches.append(cache)
    
    assert(AL.shape == (X.shape))
            
    return AL, caches

## Cost function

def compute_cost(AL, Y):
    cost = 0
    for i in range(AL.shape[0]):
    	for j in range(AL.shape[1]):
    		cost = np.square(Y.iloc[i,j] - AL.iloc[i,j])
    
    return cost




def linear_backward(dZ, cache):
	"""
	Implement the linear portion of backward propagation for a single layer (layer l)

	Arguments:
	dZ -- Gradient of the cost with respect to the linear output (of current layer l)
	cache -- tuple of values (A_prev, W, b) coming from the forward propagation in the current layer

	Returns:
	dA_prev -- Gradient of the cost with respect to the activation (of the previous layer l-1), same shape as A_prev
	dW -- Gradient of the cost with respect to W (current layer l), same shape as W
	db -- Gradient of the cost with respect to b (current layer l), same shape as b
	"""
	A_prev, W, b = cache
	m = A_prev.shape[1]
	# print("A=",A_prev.shape)
	# print("W=",W.shape)
	# print("C0=",cache[0].T.shape)
	# print("C1=",cache[1].T.shape)
	# print("dz=",dZ.shape)

	dA_prev = np.dot(cache[1].T, dZ)
	dW = np.dot(dZ, cache[0].T) / m
	db = np.squeeze(np.sum(dZ, axis=1, keepdims=True)) / m
    
	### END CODE HERE ###
	isinstance(db, float)
	assert (dA_prev.shape == A_prev.shape)
	assert (dW.shape == W.shape)
    
	return dA_prev, dW, db


def linear_activation_backward(dA, cache, activation):
    """
    Implement the backward propagation for the LINEAR->ACTIVATION layer.
    
    Arguments:
    dA -- post-activation gradient for current layer l 
    cache -- tuple of values (linear_cache, activation_cache) we store for computing backward propagation efficiently
    activation -- the activation to be used in this layer, stored as a text string: "sigmoid" or "relu"
    
    Returns:
    dA_prev -- Gradient of the cost with respect to the activation (of the previous layer l-1), same shape as A_prev
    dW -- Gradient of the cost with respect to W (current layer l), same shape as W
    db -- Gradient of the cost with respect to b (current layer l), same shape as b
    """
    linear_cache, activation_cache = cache
    
    if activation == "relu":
        dZ = relu_backward(dA, activation_cache)
        
    elif activation == "sigmoid":
        dZ = sigmoid_backward(dA, activation_cache)
    
    dA_prev, dW, db = linear_backward(dZ, linear_cache)
    
    return dA_prev, dW, db

def L_model_backward(AL, output_matrix, caches):
	"""
	Implement the backward propagation for the [LINEAR->RELU] * (L-1) -> LINEAR -> SIGMOID group
	Arguments:
	AL -- probability vector, output of the forward propagation (L_model_forward())
	Y -- true "label" vector (containing 0 if non-cat, 1 if cat)
	caches -- list of caches containing:
				every cache of linear_activation_forward() with "relu" (it's caches[l], for l in range(L-1) i.e l = 0...L-2)
				the cache of linear_activation_forward() with "sigmoid" (it's caches[L-1])
    
	Returns:
	grads -- A dictionary with the gradients
			grads["dA" + str(l)] = ... 
			grads["dW" + str(l)] = ...
			grads["db" + str(l)] = ... 
	"""
	
	grads = {}
	Y = output_matrix 
	L = len(caches) 
	m = AL.shape[1]
	Y = Y.reshape(AL.shape) 

	dAL = (-2)*(Y-AL)
    
	current_cache = caches[-1]
	grads["dA" + str(L)], grads["dW" + str(L)], grads["db" + str(L)] = linear_backward(sigmoid_backward(dAL,current_cache[1]),current_cache[0])
	for l in reversed(range(L-1)):
        # lth layer: (RELU -> LINEAR) gradients.
        # Inputs: "grads["dA" + str(l + 2)], caches". Outputs: "grads["dA" + str(l + 1)] , grads["dW" + str(l + 1)] , grads["db" + str(l + 1)] 
		dAL_cont = grads['dA3']
		current_cache = caches[l]

		dA_prev_temp, dW_temp, db_temp = linear_backward(sigmoid_backward(dAL_cont, current_cache[1]), current_cache[0])
		grads["dA" + str(l + 1)] = dA_prev_temp
		grads["dW" + str(l + 1)] = dW_temp
		grads["db" + str(l + 1)] = db_temp

	return grads

def update_parameters(parameters, grads, learning_rate):
	"""
	Update parameters using gradient descent
	
	Arguments:
	parameters -- python dictionary containing your parameters 
	grads -- python dictionary containing your gradients, output of L_model_backward
    
	Returns:
	parameters -- python dictionary containing your updated parameters 
	              parameters["W" + str(l)] = ... 
                  parameters["b" + str(l)] = ...
	"""
    
	L = len(parameters) // 2 # number of layers in the neural network
	# Update rule for each parameter. Use a for loop.
    ### START CODE HERE ### (≈ 3 lines of code)
	for l in range(L):
		t = learning_rate * grads["db" + str(l + 1)]
		bias_list = []
		for term in t.tolist():
			temp = [term]
			bias_list.append(temp)
		
		bias_array = np.asarray(bias_list)
		
		parameters["W" + str(l + 1)] = parameters["W" + str(l + 1)] - learning_rate * grads["dW" + str(l + 1)]
		#parameters["b" + str(l + 1)] = parameters["b" + str(l + 1)] - learning_rate * grads["db" + str(l + 1)]
		parameters["b" + str(l + 1)] = parameters["b" + str(l + 1)] - bias_array
    ### END CODE HERE ###
        
	return parameters

def predict(X, parameters):
	pred_res, cache_res = L_model_forward(X, final_parameters)
	AL_post = pd.DataFrame(pred_res, index=range(X.shape[0]), columns=range(X.shape[1]))
	return AL_post

## def predict(parameters, train, test):



if __name__ == "__main__":

	dat = read_data("russell_prices.csv")
	returns_mat = dat.pct_change(axis = "columns")
	returns_mat = returns_mat.drop(columns = [0])
	
	assets = returns_mat.shape[0]
	m = 100

	layer_dims = [assets, m, m, assets]

	parameters = initialize_parameters_deep(layer_dims)

	## Test Case 1 for linear_forward and linear activation forward
	B = sigmoid(returns_mat.iloc[:,:242].values)
	A = pd.DataFrame(B[0], index=range(returns_mat.iloc[:,:242].shape[0]), columns=range(returns_mat.iloc[:,:242].shape[1]))
	AL_arr, caches = L_model_forward(A, parameters)
	AL = pd.DataFrame(AL_arr, index=range(returns_mat.iloc[:,:242].shape[0]), columns=range(returns_mat.iloc[:,:242].shape[1]))
	Y = returns_mat.iloc[:, 10:252]
	grads = L_model_backward(AL_arr, Y.values, caches)


	final_parameters = update_parameters(parameters, grads, 0.01)

	# Testing for test dataset
	Bt = sigmoid(returns_mat.iloc[:,253:494].values)
	Yt = returns_mat.iloc[:, 262:]
	At = pd.DataFrame(Bt[0], index=range(returns_mat.iloc[:,253:494].shape[0]), columns=range(returns_mat.iloc[:,253:494].shape[1]))
	ALt = predict(A, final_parameters)

	cost_pre = compute_cost(At, Y)
	cost_post = compute_cost(ALt, Y)
	print("Cost before training = ", cost_pre)
	print("Cost after training = ", cost_post)


	# Prediction for last 10 days
	Bp = sigmoid(returns_mat.iloc[:,493:].values)
	Ap = pd.DataFrame(Bp[0], index=range(returns_mat.iloc[:,493:].shape[0]), columns=range(returns_mat.iloc[:,493:].shape[1]))
	ALp = predict(Ap, final_parameters)
	colnames = ["t+"+str(i+1) for i in ALp.columns]
	ALp.columns = colnames
	print("The final prediction of the returns of the 947 securites from t+1 to t+10:")
	print(ALp)


	# print("W1 = " + str(parameters["W1"]))
	# print("b1 = " + str(parameters["b1"]))
	# print("W2 = " + str(parameters["W2"]))
	# print("b2 = " + str(parameters["b2"]))
	# print("W3 = " + str(parameters["W3"]))
	# print("b3 = " + str(parameters["b3"]))

