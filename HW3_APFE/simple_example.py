# Import modules and input data
import tensorflow as tf
import pandas as pd

def read_data(fn):
	dat = pd.read_csv(fn, header = None)
	return dat

mnist = read_data("russell_prices.csv")
mnist = mnist.pct_change(axis = "columns")
mnist = mnist.drop(columns = [0])

# Let's run a simple tensorflow session
sess = tf.Session()
a = tf.constant(2)
b = tf.constant(3)
mult = tf.multiply(a,b)
sess.run(mult)

assets = mnist.shape[0]

# Define size of each layers according to input size and output size, the size_layer2 is adjustable
size_layer1 = 100
size_layer2 = 100
size_ouput  = assets

# Choose a distribution to initialize the weights
distribution = "uniform"

if distribution == "uniform":
    W = tf.Variable(tf.random_uniform([size_layer1, size_layer2], 0,1), name="W")
elif distribution == "normal":
    W = tf.Variable(tf.random_normal([size_layer1,size_layer2], stddev=0.5), name="W")
elif distribution == "truncated_normal":
    W = tf.Variable(tf.truncated_normal([size_layer1,size_layer2], mean = 0.0, stddev=0.5), name="W")

#Set the bias variables and initialize with zeros
b = tf.Variable(tf.zeros([size_layer2]), name = "biases")


print(W)


# All variables need to be initialized at the beginning of a session
init_op = tf.global_variables_initializer()
sess = tf.Session()
sess.run(init_op)
sess.run(W)
sess.run(b)

# Another way where session can be easily managed 
with tf.Session() as sess:
    # Run the initializer
    sess.run(tf.global_variables_initializer())
    #print the random values that we sample
    print(sess.run(W))
    print(sess.run(b))

# Define a variable and the initializer for it
x = tf.Variable(tf.random_uniform([size_ouput, size_layer1], 0,1), name="x")
output = tf.matmul(x, W) + b
init_op = tf.global_variables_initializer()
sess = tf.Session()
sess.run(init_op)
sess.run(output)
print(output)

# Define a placeholder for building the network structure, we need to feed value to it later
x = tf.placeholder(tf.float32, name = "x", shape=[None, size_layer1])
# Each time we feed a batch of data into the network
batch_size = 1000

# Load next batch of training data with a specific batch_size
minibatch_x, minibatch_y = mnist.train.next_batch(batch_size)
feed_dict2 = {x:minibatch_x}
init_op = tf.global_variables_initializer()
sess = tf.Session()
sess.run(init_op)
output1 = tf.matmul(x, W) + b
# Feed data into the placeholder and get the result of output1
# sess.run returns the value of the target that we run, output1 in this case
sess.run(output1, feed_dict = feed_dict2)






