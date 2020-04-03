import matplotlib.pyplot as plt
import numpy as np

import matplotlib as mpl
mpl.rcParams.update({'font.size': 18,  'figure.figsize': [8, 6],  'figure.autolayout': True})

#np.random.seed(0)

def activation (x):
    return np.tanh(x)

def activation_deriv(x):
    return 1 - np.tanh(x)*np.tanh(x)


def XOR_NN(N_input_layer, N_hidden_layer, N_output_layer, thr, epochs, eta, inputs, expected): 
	error_v= []

	sum_error=0.0

	#Random weights
	hidden_weights = np.random.uniform(size=(N_input_layer,N_hidden_layer))
	output_weights = np.random.uniform(size=(N_hidden_layer,N_output_layer))	
	
	print("Initial hidden weights: ",end='')
	print(*hidden_weights)
	
	print("Initial output weights: ",end='')
	print(*output_weights)
	
	
	#Training algorithm
	for _ in range(epochs):
		#Forward 
		hidden_layer_activation = np.dot(inputs,hidden_weights) + thr
		hidden_layer_output = activation(hidden_layer_activation)
	
		output_layer_activation = np.dot(hidden_layer_output,output_weights) + thr
		predicted_output = activation(output_layer_activation)
	
		#Back
		error = expected - predicted_output
		
		for e in error:
			sum_error+=error[0]*error[0]*0.5
		sum_error=sum_error/len(error)
		error_v.append(sum_error)
		sum_error=0
	
		d_predicted_output = error * activation_deriv(predicted_output)
		
		error_hidden_layer = d_predicted_output.dot(output_weights.T)
		d_hidden_layer = error_hidden_layer * activation_deriv(hidden_layer_output)
	
		#Updating Weights and Biases
		output_weights +=  eta* hidden_layer_output.T.dot(d_predicted_output) 
		hidden_weights +=  eta* inputs.T.dot(d_hidden_layer) 
	
	print("Final hidden weights: ",end='')
	print(*hidden_weights)
	print("Final output weights: ",end='')
	print(*output_weights)
	

	print("\nOutput from neural network after 10,000 epochs: ",end='')
	print(*predicted_output)	

	plt.plot(error_v, label=str(thr)+"")
	plt.legend(loc=3)
	plt.show()


#******************************************************************************

def ejer_1():
	thr=1
	epochs=1000
	eta=0.1

	#Input y expected datasets
	inputs = np.array([[-1,-1],[-1,1],[1,-1],[1,1]])
	expected = np.array([[1],[-1],[-1],[1]])
	
	XOR_NN(2,2,1,thr, epochs, eta, inputs, expected)
	pass

def main():
	ejer_1()


if __name__== "__main__":
	main()