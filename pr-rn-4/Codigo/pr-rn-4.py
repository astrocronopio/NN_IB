import numpy as np 
import matplotlib.pyplot as plt
import matplotlib as mpl
mpl.rcParams.update({'font.size': 18,  'figure.figsize': [8, 6],  'figure.autolayout': True})

#Activation Function
def activation_function(x):
	return np.tanh(x)

def deriv_activ_fun(x):
	return 1-np.tanh(x)*np.tanh(x);

def E(x, y):
	return (x-y)*(x-y)

#Input y output datasets
inputs = np.array([[-1,-1],[-1,1],[1,-1],[1,1]])
exp_output = np.array([[1],[-1],[-1],[1]])

#epoch + error
epochs_v=[]
error_v=[]

epochs = 10000
eta = 0.1
input_layer, hidden_layer, output_layer = 2,2,1
hidden_w = np.random.uniform(size=(input_layer,hidden_layer))

bias = 1.0

output_w = np.random.uniform(size=(hidden_layer,output_layer))

print("\nPesos de Hidden layers iniciales: ")
print(*hidden_w)
print("\nPesos de Output layers iniciales: ")
print(*output_w)


#Training 
for _ in range(epochs):
	#Forward Propagation
	#hidden_layer_activation = np.dot(inputs,hidden_w) - bias
	hidden_layer_output = activation_function(np.dot(inputs,  hidden_w) - bias)

	#output_layer_activation = np.dot(hidden_layer_output,output_weights) -bias

	predicted_output = activation_function(np.dot(hidden_layer_output, output_w) -bias)

	#Backpropagation
	error = exp_output - predicted_output

	d_predicted_output = error * deriv_activ_fun(predicted_output)
	
	error_hidden_layer = d_predicted_output.dot(output_w.T)

	d_hidden_layer = error_hidden_layer * deriv_activ_fun(hidden_layer_output)

	#Updating Weights and Biases
	output_w += eta*hidden_layer_output.T.dot(d_predicted_output)
	hidden_w += eta*inputs.T.dot(d_hidden_layer)

print("\nPesos de Hidden finales: ")
print(*hidden_w)
print("\nPesos de Output finales: ")
print(*output_w)

print("\nSalida despues de : ")
print(*predicted_output)